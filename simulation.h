#ifndef SIMULATION_H
#define SIMULATION_H

#include <SDL2/SDL.h>
#include <stdbool.h>

// Dimensions of the simulation
typedef struct Size{
    int width;
    int height;
} Size;

// Offset for drawing operations
typedef struct Offset{
    int x;
    int y;
} Offset;

// Delayed commands that must only be executed at
// the end of a simulation loop
// (it is used to load a new simulation)
typedef enum command{
    no_command,
    load
} command;

// Cell state
enum cell_state{
    empty,  // Inactive (empty)
    active  // Active
};

// Simulation properties
typedef struct Simulation{
    Size size;         // Number of cells in a row and in a column
    Offset offset;     // Offset of the map from the top left corner
    int speed;         // Number of steps in the simulation in a second
    bool running;      // The simulation is running
    bool firstStart;   // Is this the first start
                       // (If it is true, then it should save the default map before playing or stepping)
    command command;   // Delayed commands that must only be executed at
                       // the end of a simulation loop, because it modifies internal data structures
    int zoom;          // Level of zoom ( the size of a cell in pixels )
    int ** map;        // Visible map
    int ** tempMap;    // Auxiliary map to calculate next state
    int ** defaultMap; // Default state ( before the simulation is started )
} Simulation;

// Clear the map
void clearMap(Simulation * sim, int ** map);

// Calculate how many neighbours the given cell has
void recordNeighbourCells(Simulation * sim, int cellX, int cellY);

// To calculate the next simulation state,
// count how many neighbours each cell has
void countNeighbourCells(Simulation * sim);

// Advance the simulation to the next state
void cycle(Simulation * sim);

// Copy the content of the map to another
// (it is used to calculate next state on an auxiliary map)
void copyMap(Simulation * sim, int ** dst, int ** src);

// Save current map as checkpoint
void setAsDefaultMap(Simulation * sim);

// Restore checkpoint
void restoreDefaultMap(Simulation * sim);

// Set simulation speed given by the speed slider
void setSpeedSlider(Simulation * sim);

// Timer to create continuous simulation
Uint32 simulationStepper(Uint32 interval, void * param);

// Initialize the simulation structure
// Return: Simulation
Simulation simulation_init(int width, int height);

// Frees the memory allocated by the simulation
void simulation_free(Simulation * sim);

// Frees the simulation structure and stops the SDL timer
void simulation_destroy(Simulation * sim, SDL_TimerID timer);

// Initialize a new simulation with the given dimensions
void simulation_reinit(Simulation * sim, SDL_TimerID * timer, int width, int height);

// Prompt the user to enter the dimensions and then
// create the simulation
// Return: Simulation
Simulation simulation_create();

#endif