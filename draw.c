#include <SDL2/SDL.h>
#include <stdio.h>

#include "draw.h"

// Set the color of the next drawing operation
void setDrawColor(SDL_Renderer * renderer, SDL_Color const * color){
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
}

// Draw a line between the two points with the given offset
void drawLineWithOffset(SDL_Renderer * renderer, Offset * offset, int x1, int y1, int x2, int y2){
    SDL_RenderDrawLine(renderer, x1 + offset->x, y1 + offset->y, x2 + offset->x, y2 + offset->y);
}

// Draw a rectangle with offset
void drawRectWithOffset(SDL_Renderer * renderer, Offset * offset, SDL_Rect * area){
    area->x += offset->x;
    area->y += offset->y;
    SDL_RenderFillRect(renderer, area);
}

// Get the area of the map on the screen
// Return: SDL_Rect
SDL_Rect viewport(Simulation * sim){
    SDL_Rect area;
    area.x = sim->offset.x;
    area.y = sim->offset.y;
    area.w = sim->size.width * (sim->zoom + 1);
    area.h = sim->size.height  * (sim->zoom + 1);
    return area;
}

// Fill the screen with color_background
void clearWindow(SDL_Renderer * renderer){
    setDrawColor(renderer, &color_background);
    SDL_RenderClear(renderer);
}

// Clear given area of the screen
void clearArea(SDL_Renderer * renderer, const SDL_Rect * area){
    SDL_RenderFillRect(renderer, area);
}

// Draw the grid lines
void drawGrid(SDL_Renderer * renderer, Simulation * sim){
    int width, height, zoom;
    int px, py;
    int maxWidth, maxHeight;
    SDL_Rect simulationAreaOnScreen = viewport(sim);
    width = sim->size.width;
    height  = sim->size.height;
    px = 0;
    py = 0;
    zoom  = sim->zoom;
    maxHeight  = (zoom * height ) + height;
    maxWidth = (zoom * width) + width;
    
    setDrawColor(renderer, &color_white);
    clearArea(renderer, &simulationAreaOnScreen);
    setDrawColor(renderer, &color_black);
    
    for(int i = 0; i < width + 1; i++){
        drawLineWithOffset(renderer, &sim->offset, px, 0, px, maxHeight);
        px += zoom + 1;
    }
    for(int i = 0; i < height + 1; i++){
        drawLineWithOffset(renderer, &sim->offset, 0, py, maxWidth, py);
        py += zoom + 1;
    }
}

// Draw the cell in the given row and column
void drawCell(SDL_Renderer * renderer, Simulation * sim, int row, int column){
    SDL_Rect area;
    area.y = row * (sim->zoom + 1);
    area.x = column * (sim->zoom + 1);
    area.w = sim->zoom + 1;
    area.h = sim->zoom + 1;
    drawRectWithOffset(renderer, &sim->offset, &area);
}

// Iterate over all the cells and draw them
void drawCells(SDL_Renderer * renderer, Simulation * sim){
    int ** map = sim->map;
    for(int r = 0; r < sim->size.height; r++){
        for(int c = 0; c < sim->size.width; c++){
            if(map[r][c]){
                drawCell(renderer, sim, r, c);
            }
        }
    }
}

// Draw a button
void drawButton(SDL_Renderer * renderer, Button * button){
    setDrawColor(renderer, &color_white);
    SDL_RenderFillRect(renderer, &(button->area));
    SDL_RenderDrawRect(renderer, &(button->area));
    SDL_RenderCopy(renderer, button->label, NULL, &(button->labelPosition));
}

// Draw the "Speed:" label
void drawSpeedLabel(SDL_Renderer * renderer){
    SDL_RenderCopy(renderer, speedLabel, NULL, &speedLabelPosition);
}

// Draw the speed slider
void drawSpeedChanger(SDL_Renderer * renderer, Simulation * sim){
    char text[4] = {0};
    SDL_Rect labelPosition;

    SDL_Rect slider = (SDL_Rect){  0, 475,  20,  20};
    slider.x = speedBar.x + (int)(((speedBar.w-slider.w) / (double)(50-1)) * (sim->speed - 1));
    setDrawColor(renderer, &color_background);
    SDL_RenderFillRect(renderer, &speedBar);
    setDrawColor(renderer, &color_speed_indicator);
    SDL_RenderFillRect(renderer, &slider);
    
    snprintf(text, 4, "%d", sim->speed);
    SDL_Texture * label = initText(renderer, text, &color_white, &color_speed_indicator);
    SDL_QueryTexture(label, NULL, NULL, &labelPosition.w, &labelPosition.h);
    labelPosition.x = slider.x + ((slider.w - labelPosition.w) / 2);
    labelPosition.y = slider.y + ((slider.h - labelPosition.h) / 2);
    SDL_RenderCopy(renderer, label, NULL, &labelPosition);
    SDL_DestroyTexture(label);
}

// Draw all the menu components
// Components: Background color, Buttons, Texts, Speed Slider
void drawMenu(SDL_Renderer * renderer, Simulation * sim, Button buttons[], int numOfButtons){
    setDrawColor(renderer, &color_menu);
    clearArea(renderer, &menu_area);
    
    for(int i = 0; i < numOfButtons; i++){
        drawButton(renderer, &(buttons[i]));
    }
    
    drawSpeedLabel(renderer);
    drawSpeedChanger(renderer, sim);
}

// Renders the current frame
void renderFrame(SDL_Renderer * renderer, Simulation * sim, Button buttons[], int numOfButtons){
    clearWindow(renderer);
    drawGrid(renderer, sim);
    drawCells(renderer, sim);
    drawMenu(renderer, sim, buttons, numOfButtons);
    SDL_RenderPresent(renderer);
}