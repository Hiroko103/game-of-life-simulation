#ifndef FILE_H
#define FILE_H

#include <SDL2/SDL.h>
#include "simulation.h"

// Save the current state of the simulation
void saveSimulationToFile(Simulation * sim);

// Load simulation from file
void loadSimulationFromFile(Simulation * sim, SDL_TimerID * timer);

#endif