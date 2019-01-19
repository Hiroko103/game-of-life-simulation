#include <stdio.h>
#include "error.h"
#include "simulation.h"
#include "userInterface.h"

// Clear the map
void clearMap(Simulation * sim, int ** map){
    for(int i = 0; i < sim->size.height; i++){
        memset(map[i], 0, sim->size.width * sizeof(int));
    }
}

// Calculate how many neighbours the given cell has
void recordNeighbourCells(Simulation * sim, int cellX, int cellY){
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(i != 0 || j != 0){
                if(cellX + i >= 0 && cellX + i < sim->size.width &&
                   cellY + j >= 0 && cellY + j < sim->size.height){
                    sim->tempMap[cellY + j][cellX + i]++;
                }
            }
        }
    }
}

// To calculate the next simulation state,
// count how many neighbours each cell has
void countNeighbourCells(Simulation * sim){
    clearMap(sim, sim->tempMap);
    for(int y = 0; y < sim->size.height; y++){
        for(int x = 0; x < sim->size.width; x++){
            if(sim->map[y][x] == active){
                recordNeighbourCells(sim, x, y);
            }
        }
    }
}

// Advance the simulation to the next state
void cycle(Simulation * sim){
    int numOfNeighbour;
    
    countNeighbourCells(sim);
    for(int y = 0; y < sim->size.height; y++){
        for(int x = 0; x < sim->size.width; x++){
            numOfNeighbour = sim->tempMap[y][x];
            if(numOfNeighbour >= 2 && numOfNeighbour <= 3){
                // If the cell has two neighbours, the cell will remain unchanged
                if(numOfNeighbour == 3){
                    sim->map[y][x] = active;
                }
            } else {
                sim->map[y][x] = empty;
            }
        }
    }
}

// Copy the content of the map to another
// (it is used to calculate next state on an auxiliary map)
void copyMap(Simulation * sim, int ** dst, int ** src){
    for(int i = 0; i < sim->size.height; i++){
        memcpy(dst[i], src[i], sim->size.width * sizeof(int));
    }
}

// Save current map as checkpoint
void setAsDefaultMap(Simulation * sim){
    copyMap(sim, sim->defaultMap, sim->map);
}

// Restore checkpoint
void restoreDefaultMap(Simulation * sim){
    copyMap(sim, sim->map, sim->defaultMap);
}

// Set simulation speed given by the speed slider
void setSpeedSlider(Simulation * sim){
    int mouseX, mouseY, speed;
    SDL_GetMouseState(&mouseX, &mouseY);
    speed = (mouseX - speedBar.x) / (speedBar.w / 50) + 1;
    if(speed < 1){
        speed = 1;
    } else if(speed > 50){
        speed = 50;
    }
    sim->speed = speed;
}

// Timer to create continuous simulation
Uint32 simulationStepper(Uint32 interval, void * param){
    Simulation * sim = (Simulation*)param;
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = 1;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return (Uint32)(1 / (double)sim->speed * 1000);
}

// Initialize the simulation structure
// Return: Simulation
Simulation simulation_init(int width, int height){
    Simulation sim;
    sim.size = (Size){width, height};
    sim.offset = (Offset){0,0};
    sim.running = false;
    sim.firstStart = true;
    sim.command = no_command;
    sim.speed = 1;
    sim.zoom = 11;
    sim.map = malloc(sim.size.height * sizeof(int *));
    if(sim.map == NULL){
        notEnoughMemory();
    }
    sim.tempMap = malloc(sim.size.height * sizeof(int *));
    if(sim.tempMap == NULL){
        notEnoughMemory();
    }
    sim.defaultMap = malloc(sim.size.height * sizeof(int *));
    if(sim.defaultMap == NULL){
        notEnoughMemory();
    }
    for(int i = 0; i < sim.size.height; i++){
        sim.map[i] = calloc(sim.size.width, sizeof(int));
        if(sim.map[i] == NULL){
            notEnoughMemory();
        }
        sim.tempMap[i] = calloc(sim.size.width, sizeof(int));
        if(sim.tempMap[i] == NULL){
            notEnoughMemory();
        }
        sim.defaultMap[i] = calloc(sim.size.width, sizeof(int));
        if(sim.defaultMap[i] == NULL){
            notEnoughMemory();
        }
    }
    return sim;
}

// Frees the memory allocated by the simulation
void simulation_free(Simulation * sim){
    for(int i = 0; i < sim->size.height; i++){
        free(sim->map[i]);
        free(sim->tempMap[i]);
        free(sim->defaultMap[i]);
    }
    free(sim->defaultMap);
    free(sim->tempMap);
    free(sim->map);
}

// Frees the simulation structure and stops the SDL timer
void simulation_destroy(Simulation * sim, SDL_TimerID timer){
    SDL_RemoveTimer(timer);
    simulation_free(sim);
}

// Initialize a new simulation with the given dimensions
void simulation_reinit(Simulation * sim, SDL_TimerID * timer, int width, int height){
    simulation_destroy(sim, *timer);
    *sim = simulation_init(width, height);
    *timer = SDL_AddTimer((Uint32)(1 / (double)sim->speed * 1000), simulationStepper, sim);
}

// Prompt the user to enter the dimensions and then
// create the simulation
// Return: Simulation
Simulation simulation_create(){
    int width, height;
    printf("Enter the dimensions!\n");
    printf("Width: ");
    scanf("%d", &width);
    printf("Height: ");
    scanf("%d", &height);
    
    return simulation_init(width, height);
}