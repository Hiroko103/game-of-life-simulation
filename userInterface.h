#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <SDL2/SDL.h>
#include "simulation.h"

// Button identifiers
typedef enum button_type{
    btn_step,  // Step
    btn_start, // Start
    btn_stop,  // Stop
    btn_reset, // Restore default state
    btn_save,  // Save
    btn_load   // Load
} button_type;

// Button properties
typedef struct Button{
    SDL_Rect area;           // Position and size
    button_type type;        // Button identifier
    SDL_Rect labelPosition;  // Alignment of text (centered)
    SDL_Texture * label;     // Texture generated from text
} Button;

// Pre-generated text
SDL_Texture * speedLabel; // "Speed: " label on the menu

// Position and sizes for user interface components
const SDL_Rect menu_area;
const SDL_Rect speedBar;
      SDL_Rect speedLabelPosition;

// User interface component colors
const SDL_Color color_background;
const SDL_Color color_white;
const SDL_Color color_black;
const SDL_Color color_menu;
const SDL_Color color_speed_indicator;

// Pointed cell by the cursor
// Return: If the cursor is on a cell TRUE, otherwise FALSE
bool pointedCell(Simulation * sim, int * x, int * y);

// Checks if the user clicked in the given area
// Return: TRUE, if the user clicked in the area, FALSE otherwise
bool areaClicked(const SDL_Rect * area);

// Checks if the user click on the menu
// Return: TRUE, if the user clicked the menu, FALSE otherwise
bool userClickedOnMenu();

// Checks if the user wants to edit a cell (right or left mouse button active)
// Return: If a cell was modified TRUE, FALSE otherwise
bool checkForEditing(Simulation * sim);

// Get which button the user clicked
// Return: The clicked Button, or NULL if no button was clicked
Button * clickedButton(Button buttons[], int numOfButtons);

// Generate a texture from a text
// Return: Generated texture
SDL_Texture * initText(SDL_Renderer * renderer, char * text, const SDL_Color * fg, const SDL_Color * bg);

// Generate the text and center align it on the button
// Return: Button
Button initButton(SDL_Renderer * renderer, SDL_Rect area, char * text, button_type type);

// Initialize all the buttons (position, size, text)
void initButtons(SDL_Renderer * renderer, Button buttons[]);

// Pre-generate static texts
void generateLabel(SDL_Renderer * renderer);

// Handle button events
// Return: TRUE if a button was clicked, FALSE otherwise
bool buttonHandler(Button buttons[], int numOfButtons, Simulation * sim);

#endif