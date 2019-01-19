#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "error.h"
#include "simulation.h"
#include "userInterface.h"
#include "draw.h"
#include "file.h"

// After a failed attempt to allocate memory,
// print error message and exit the program
void notEnoughMemory(){
    printf("Memory allocation failed!\n");
    exit(0);
}

int main(int argc, char *argv[]){
    SDL_TimerID timer;
    SDL_Event ev;
    Simulation sim;
    const Uint8 * keyboardState = NULL;
    bool updateFrame = false;
    bool running = true;
    bool speedSliderDragged = false;
    int numOfButtons = 6;
    Button buttons[numOfButtons];
    
    sim = simulation_create();
    
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        return 0;
    }
    TTF_Init();
    
    keyboardState = SDL_GetKeyboardState(NULL);
    timer = SDL_AddTimer((Uint32)(1 / (double)sim.speed * 1000), simulationStepper, &sim);
    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;
    
    if(SDL_CreateWindowAndRenderer(800, 600, 0, &window, &renderer) != 0){
        return 0;
    }
    SDL_SetWindowTitle(window, "Conway's Game of Life");
    initButtons(renderer, buttons);
    generateLabel(renderer);
    renderFrame(renderer, &sim, buttons, numOfButtons);
    
    while(running){
        SDL_WaitEvent(&ev);
        if(ev.type == SDL_QUIT){
            // Exit
            SDL_Quit();
            running = false;
        } else if(ev.type == SDL_MOUSEWHEEL){
            // Zooming
            updateFrame = true;
            if(ev.wheel.y > 0)
            {  
                sim.zoom++;
            }
            else if(ev.wheel.y < 0)
            {
                if(sim.zoom > 1){
                    sim.zoom--;
                }
            }
        } else if(ev.type == SDL_MOUSEMOTION){
            if(!userClickedOnMenu()){
                updateFrame |= checkForEditing(&sim);
            }
            // Move view when SPACE is held down and 
            // the cursor is moving
            if(keyboardState[SDL_SCANCODE_SPACE]){
                sim.offset.x += ev.motion.xrel;
                sim.offset.y += ev.motion.yrel;
                updateFrame = true;
            }
            if(speedSliderDragged && areaClicked(&speedBar)){
                setSpeedSlider(&sim);
                updateFrame = true;
            }
        } else if(ev.type == SDL_MOUSEBUTTONDOWN){
            if(userClickedOnMenu()){
                // Detect clicks on buttons
                updateFrame |= buttonHandler(buttons, numOfButtons, &sim);
                // Detect click on the speed slider
                if(areaClicked(&speedBar)){
                    setSpeedSlider(&sim);
                    updateFrame = true;
                    speedSliderDragged = true;
                }
            } else {
                // Editing cell state
                updateFrame |= checkForEditing(&sim);                
            }
        } else if(ev.type == SDL_MOUSEBUTTONUP){
            speedSliderDragged = false;
        } else if(ev.type == SDL_USEREVENT){
            // Timer
            // Step to next frame
            if(ev.user.code == 1){
                if(sim.running){
                    cycle(&sim);
                    updateFrame = true;
                }
            }
        }
        if(sim.command == load){
            loadSimulationFromFile(&sim, &timer);
            sim.command = no_command;
        }
        if(updateFrame){
            // If the user edited a cell or the timer fired, then
            // render the next frame
            renderFrame(renderer, &sim, buttons, numOfButtons);
            updateFrame = false;
        }
    }
    TTF_Quit();
    simulation_destroy(&sim, timer);

    return 0;
}