#ifndef DRAW_H
#define DRAW_H

#include <SDL2/SDL.h>
#include "simulation.h"
#include "userInterface.h"

// Set the color of the next drawing operation
void setDrawColor(SDL_Renderer * renderer, SDL_Color const * color);

// Draw a line between the two points with the given offset
void drawLineWithOffset(SDL_Renderer * renderer, Offset * offset, int x1, int y1, int x2, int y2);

// Draw a rectangle with offset
void drawRectWithOffset(SDL_Renderer * renderer, Offset * offset, SDL_Rect * area);

// Get the area of the map on the screen
// Return: SDL_Rect
SDL_Rect viewport(Simulation * sim);

// Fill the screen with color_background
void clearWindow(SDL_Renderer * renderer);

// Clear given area of the screen
void clearArea(SDL_Renderer * renderer, const SDL_Rect * area);

// Draw the grid lines
void drawGrid(SDL_Renderer * renderer, Simulation * sim);

// Draw the cell in the given row and column
void drawCell(SDL_Renderer * renderer, Simulation * sim, int row, int column);

// Iterate over all the cells and draw them
void drawCells(SDL_Renderer * renderer, Simulation * sim);

// Draw a button
void drawButton(SDL_Renderer * renderer, Button * button);

// Draw the "Speed:" label
void drawSpeedLabel(SDL_Renderer * renderer);

// Draw the speed slider
void drawSpeedChanger(SDL_Renderer * renderer, Simulation * sim);

// Draw all the menu components
// Components: Background color, Buttons, Texts, Speed Slider
void drawMenu(SDL_Renderer * renderer, Simulation * sim, Button buttons[], int numOfButtons);

// Renders the current frame
void renderFrame(SDL_Renderer * renderer, Simulation * sim, Button buttons[], int numOfButtons);

#endif