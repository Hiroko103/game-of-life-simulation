#include <stdio.h>
#include "file.h"

// Save the current state of the simulation
void saveSimulationToFile(Simulation * sim){
    FILE * fp = fopen("map.bin", "wb");
    unsigned char bitEncodedField;
    unsigned char bitCounter;
    
    fprintf(fp, "%dx%d\n", sim->size.width, sim->size.height);
    fprintf(fp, "%d\n", sim->speed);
    for(int i = 0; i < sim->size.height; i++){
        bitEncodedField = 0x00;
        bitCounter = 0;
        for(int j = 0; j < sim->size.width; j++){
            if(sim->map[i][j] == 1){
                bitEncodedField |= (0x1 << bitCounter);
            }
            bitCounter++;
            if(bitCounter > 7){
                fputc(bitEncodedField, fp);
                bitEncodedField = 0x00;
                bitCounter = 0;
            }
        }
        if(bitCounter != 7){
            fputc(bitEncodedField, fp);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

// Load simulation from file
void loadSimulationFromFile(Simulation * sim, SDL_TimerID * timer){
    FILE * fp = fopen("map.bin", "rb");
    unsigned char bitEncodedField;
    int width, height, speed;
    unsigned char bit;
    
    if(fp == NULL){
        printf("Error when loading map.\n(map.bin doesn't exist)\n");
        return;
    }
    
    fscanf(fp, "%dx%d\n", &width, &height);
    fscanf(fp, "%d\n", &speed);
    
    simulation_reinit(sim, timer, width, height);
    sim->speed = speed;
    
    for(int i = 0; i < height; i++){
        bit = 0;
        for(int j = 0; j < width; j++){
            if(bit == 0){
                bitEncodedField = fgetc(fp);
            }
            sim->map[i][j] = ((bitEncodedField >> bit) & 0x1);
            bit++;
            if(bit == 8){
                bit = 0;
            }
        }
        fgetc(fp); // Skip new line character
    }
    
    setAsDefaultMap(sim);
    
    fclose(fp);
}