# Bouncing Tumbling Cube

## Overview
This project is a simple, visuall C program that uses the SDL2 library to render a 3D cube that bounces and tumbles around the screen. It's designed as a practice project to explore graphics programming in C, specifically focusing on 3D transformations, event handling, and rendering techniques with SDL2. This project is purely for fun and to get some more practice with C.

## Features
- **3D Cube Rendering**: Utilizes basic 3D to 2D point projection to render a cube that can be rotated along the X and Y axes.
- **Bouncing and Tumbling**: The cube bounces around the screen, continuously tumbling as it moves.
- **Interactive Controls**: Users can interact with the program using keyboard inputs to change the line thickness of the cube, generate a random light color for the cube lines, and quit the application.

## How It Works
The program initializes an SDL window and renderer, then enters a game loop where it handles user input, updates the cube's rotation and position, and renders the cube frame by frame. The cube's vertices are defined in 3D space, and edges connect these vertices to form the cube. Each frame, the program rotates the cube's vertices around the X and Y axes, projects these 3D coordinates to 2D screen space, and then draws lines between the projected points to render the cube. Additionally, the program allows users to interact with it through keyboard to do basic things like change the color and thickness of the lines that make up the cube. I'm working on adding more fun little ways to interact with the cube, like being able to grab it and throw it around, or speed up or slow down the tumbling.

## Dependencies
- SDL2: For creating the window, handling events, and rendering.
- SDL_ttf: For rendering text on the screen.

## Building and Running
To build and run this project, you need to have SDL2 and SDL_ttf installed on your system. Once installed, you can compile the program using a C compiler like gcc, linking against the SDL2 and SDL_ttf libraries. For example:

```bash
gcc main.c -o BouncingCube -lSDL2 -lSDL2_ttf
./BouncingCube
```