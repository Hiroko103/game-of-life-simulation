# Conway's Game of Life simulator

Conway's Game of Life simulator written in C using SDL2.

This project was a submission for the first semester's programming course in university.

## Build command

```
gcc -Wall -m32 simulation.c draw.c userInterface.c file.c main.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
```

## Try it out!

You can try it out without compiling the program by clicking [here](https://github.com/Hiroko103/game-of-life-simulation/releases/download/v1.0/simulator.zip).

The archive includes the executable and the SDL dependencies.

## Features

* No limit on the size of the map
* Zoom
* The map can be edited while running
* Start and stop simulation
* Step-by-step mode
* Simulation reset
* Save/Load simulation state (`map.bin` file)
* Speed control (in the range between 1-50)

## Controls

**Left click:** Activate cell

**Right click:** Deactivate (remove) cell

**Zoom:** Use mouse wheel scroll

**Moving view:** Hold down SPACE and then move the cursor

## Screenshot

<p align="center">
  <img width="800" height="600" src="https://github.com/Hiroko103/game-of-life-simulation/blob/master/screenshot.png">
</p>

## License

This software is released under the MIT license.

>  A short and simple permissive license with conditions only requiring preservation of copyright and license notices. Licensed works, modifications, and larger works may be distributed under different terms and without source code.

Refer to the [LICENSE](https://github.com/Hiroko103/game-of-life-simulation/blob/master/LICENSE) file for the complete text.
