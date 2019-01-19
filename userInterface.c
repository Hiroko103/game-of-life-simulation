#include <stdio.h>
#include <SDL2/SDL_ttf.h>

#include "simulation.h"
#include "userInterface.h"
#include "file.h"

// Position and sizes for user interface components
const SDL_Rect menu_area = (SDL_Rect){600,   0, 200, 600};
const SDL_Rect speedBar  = (SDL_Rect){625, 475, 150,  20};
      SDL_Rect speedLabelPosition  = (SDL_Rect){625, 450, 0,  0};

// User interface component colors
const SDL_Color color_background      = (SDL_Color){ 44,  62,  80, SDL_ALPHA_OPAQUE};
const SDL_Color color_white           = (SDL_Color){255, 255, 255, SDL_ALPHA_OPAQUE};
const SDL_Color color_black           = (SDL_Color){  0,   0,   0, SDL_ALPHA_OPAQUE};
const SDL_Color color_menu            = (SDL_Color){ 52,  73,  94, SDL_ALPHA_OPAQUE};
const SDL_Color color_speed_indicator = (SDL_Color){230, 126,  34, SDL_ALPHA_OPAQUE};

// Pointed cell by the cursor
// Return: If the cursor is on a cell TRUE, otherwise FALSE
bool pointedCell(Simulation * sim, int * x, int * y){
    *x -= sim->offset.x;
    *y -= sim->offset.y;
    *x = *x / (sim->zoom + 1);
    *y = *y / (sim->zoom + 1);
    if(*x >= 0 && *x < sim->size.width &&
       *y >= 0 && *y < sim->size.height){
        return true;
    }
    return false;
}

// Checks if the user clicked in the given area
// Return: TRUE, if the user clicked in the area, FALSE otherwise
bool areaClicked(const SDL_Rect * area){
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    if(mouseX >= area->x && mouseX <= (area->x + area->w) &&
       mouseY >= area->y && mouseY <= (area->y + area->h)){
        return true;
    }
    return false;
}

// Checks if the user click on the menu
// Return: TRUE, if the user clicked the menu, FALSE otherwise
bool userClickedOnMenu(){
    return areaClicked(&menu_area);
}

// Checks if the user wants to edit a cell (right or left mouse button active)
// Return: If a cell was modified TRUE, FALSE otherwise
bool checkForEditing(Simulation * sim){
    int mouseX, mouseY;
    Uint32 state;
    state = SDL_GetMouseState(&mouseX, &mouseY);
    bool validCell = pointedCell(sim, &mouseX, &mouseY);
    
    if(state & SDL_BUTTON(SDL_BUTTON_LEFT)){
        if(validCell){
            sim->map[mouseY][mouseX] = active;
            return true;
        }
    } else if(state & SDL_BUTTON(SDL_BUTTON_RIGHT)){
        if(validCell){
            sim->map[mouseY][mouseX] = empty;
            return true;
        }
    }
    return false;
}

// Get which button the user clicked
// Return: The clicked Button, or NULL if no button was clicked
Button * clickedButton(Button buttons[], int numOfButtons){
    int mouseX, mouseY;
    Button * button;
    SDL_GetMouseState(&mouseX, &mouseY);
    for(int i = 0; i < numOfButtons; i++){
        button = &(buttons[i]);
        if(areaClicked(&(button->area))){
            return button;
        }
    }
    return NULL;
}

// Generate a texture from a text
// Return: Generated texture
SDL_Texture * initText(SDL_Renderer * renderer, char * text, const SDL_Color * fg, const SDL_Color * bg){
    TTF_Font * font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 12);
    if(font == NULL) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }
    SDL_Surface * textSurface;
    textSurface = TTF_RenderUTF8_Shaded(font, text, *fg, *bg);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
    return texture;
}

// Generate the text and center align it on the button
// Return: Button
Button initButton(SDL_Renderer * renderer, SDL_Rect area, char * text, button_type type){
    Button button;
    button.area = area;
    button.type = type;
    button.label = initText(renderer, text, &color_black, &color_white);
    SDL_QueryTexture(button.label, NULL, NULL, &button.labelPosition.w, &button.labelPosition.h);
    button.labelPosition.x = area.x + ((area.w - button.labelPosition.w) / 2);
    button.labelPosition.y = area.y + ((area.h - button.labelPosition.h) / 2);
    return button;
}

// Initialize all the buttons (position, size, text)
void initButtons(SDL_Renderer * renderer, Button buttons[]){
    buttons[0] = initButton(renderer, (SDL_Rect){625, 30,  150, 50}, "Step",    btn_step);
    buttons[1] = initButton(renderer, (SDL_Rect){625, 100, 150, 50}, "Start",   btn_start);
    buttons[2] = initButton(renderer, (SDL_Rect){625, 170, 150, 50}, "Stop",    btn_stop);
    buttons[3] = initButton(renderer, (SDL_Rect){625, 240, 150, 50}, "Reset",   btn_reset);
    buttons[4] = initButton(renderer, (SDL_Rect){625, 310, 150, 50}, "Save",    btn_save);
    buttons[5] = initButton(renderer, (SDL_Rect){625, 380, 150, 50}, "Load",    btn_load);
}

// Pre-generate static texts
void generateLabel(SDL_Renderer * renderer){
    speedLabel = initText(renderer, "Speed: ", &color_white, &color_menu);
    SDL_QueryTexture(speedLabel, NULL, NULL, &speedLabelPosition.w, &speedLabelPosition.h);
}

// Handle button events
// Return: TRUE if a button was clicked, FALSE otherwise
bool buttonHandler(Button buttons[], int numOfButtons, Simulation * sim){
    Button * button;
    button = clickedButton(buttons, numOfButtons);
    if(button == NULL){
        return false;
    }
    switch(button->type){
        case btn_step:
            // Step simulation
            if(sim->firstStart){
                setAsDefaultMap(sim);
                sim->firstStart = false;
            }
            cycle(sim);
            break;
        case btn_start:
            // Start simulation
            if(sim->firstStart){
                setAsDefaultMap(sim);
                sim->firstStart = false;
            }
            sim->running = true;
            break;
        case btn_stop:
            // Stop simulation
            sim->running = false;
            break;
        case btn_reset:
            // Restore default state
            sim->running = false;
            sim->firstStart = true;
            restoreDefaultMap(sim);
            break;
        case btn_save:
            // Save simulation
            saveSimulationToFile(sim);
            break;
        case btn_load:
            // Load simulation
            sim->command = load;
            break;
    }
    return true;
}