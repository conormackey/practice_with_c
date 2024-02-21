#include <SDL.h>
#include "types.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <SDL_ttf.h>
#include "graphics.h"
#include "utils.h"
#include "text.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// vertices of a cube
Point3D cubeVertices[8] = {
    {-50, -50, -50}, {50, -50, -50}, {50, 50, -50}, {-50, 50, -50},
    {-50, -50, 50}, {50, -50, 50}, {50, 50, 50}, {-50, 50, 50}
};

// edges of the cube, connecting vertices
int cubeEdges[12][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0},
    {4, 5}, {5, 6}, {6, 7}, {7, 4},
    {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

float posX = WINDOW_WIDTH / 2, posY = WINDOW_HEIGHT / 2; // starting position
float velX = 2.0, velY = 2.0; // movement speed
int lineThickness = 1; // initial line thickness
float cubeScale = 1.0; // initial cube scale

// rotates a point around the X axis
void rotateX(Point3D* point, float angle) {
    float y = point->y;
    point->y = cos(angle) * y - sin(angle) * point->z;
    point->z = sin(angle) * y + cos(angle) * point->z;
}

// rotates a point around the Y axis
void rotateY(Point3D* point, float angle) {
    float x = point->x;
    point->x = cos(angle) * x + sin(angle) * point->z;
    point->z = -sin(angle) * x + cos(angle) * point->z;
}

// scales a point by the cubeScale factor
void scalePoint(Point3D* point) {
    point->x *= cubeScale;
    point->y *= cubeScale;
    point->z *= cubeScale;
}

float angleX = 0.0, angleY = 0.0; // rotation angles
float angleIncrementX = 0.01, angleIncrementY = 0.01; // rotation speed or increment

int main(int argc, char* argv[]) {
    SDL_Window* window; // window handle
    SDL_Renderer* renderer; // renderer handle
    srand(time(NULL));  // random number generator

    SDL_Init(SDL_INIT_VIDEO); // initialize sdl video subsystem
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }

    // font
    TTF_Font* font = TTF_OpenFont("./terminal-grotesque.ttf", 18);
    if (!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(2);
    }

    // create a window with a title, default position, and defined width and height
    window = SDL_CreateWindow("Bouncing Tumbling Cube", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    // create a renderer for the window with hardware acceleration
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int running = 1; // game loop flag
    SDL_Event event; // event variable
    while (running) {
        // event loop
        while (SDL_PollEvent(&event)) {
            // check for quit event
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            // check for key press event
            else if (event.type == SDL_KEYDOWN) {
                // check if the 'q' key is pressed
                if (event.key.keysym.sym == SDLK_q) {
                    running = 0;
                }
                // increase line thickness when 'i' is pressed
                else if (event.key.keysym.sym == SDLK_i) {
                    lineThickness += 1; // Increase thickness
                }
                // decrease line thickness when 'o' is pressed, ensuring it doesn't go below 1
                else if (event.key.keysym.sym == SDLK_o && lineThickness > 1) {
                    lineThickness -= 1; // Decrease thickness
                }
                // change line color when 'l' is pressed
                else if (event.key.keysym.sym == SDLK_l) {
                    generateRandomLightColor(); // Generate a new light color
                }
                // increase rotation speed when 'j' is pressed
                else if (event.key.keysym.sym == SDLK_j) {
                    angleIncrementX += 0.005; // increase rotation speed along X axis
                    angleIncrementY += 0.005; // increase rotation speed along Y axis
                }
                // decrease rotation speed when 'k' is pressed, ensuring it doesn't become too slow or negative
                else if (event.key.keysym.sym == SDLK_k) {
                    if (angleIncrementX > 0.005 && angleIncrementY > 0.005) { // ensure speed doesn't become too slow or negative
                        angleIncrementX -= 0.005; // decrease rotation speed along X axis
                        angleIncrementY -= 0.005; // decrease rotation speed along Y axis
                    }
                }
                // grow the cube when 'n' is pressed
                else if (event.key.keysym.sym == SDLK_n) {
                    cubeScale += 0.1; // increase cube scale
                }
                // shrink the cube when 'm' is pressed, ensuring it doesn't become too small
                else if (event.key.keysym.sym == SDLK_m && cubeScale > 0.1) {
                    cubeScale -= 0.1; // decrease cube scale
                }
                // toggle smooth color transition mode when 'p' is pressed
                else if (event.key.keysym.sym == SDLK_p) {
                    toggleColorChangeMode(); // Toggle the smooth color transition mode
                }
            }
        }

        // update cube rotation
        angleX += angleIncrementX;
        angleY += angleIncrementY;

        // update cube position
        posX += velX;
        posY += velY;

        // cube "radius" in the x and y dimensions
        float cubeRadiusX = 100; // Half the cube's width
        float cubeRadiusY = 100; // Half the cube's height

        // check for collisions with window boundaries, adjusted for cube size
        // if the cube hits the left or right edge of the window, reverse the x velocity
        if (posX - cubeRadiusX <= 0 || posX + cubeRadiusX >= WINDOW_WIDTH) {
            velX = -velX;
            // Adjust position to prevent sticking to the edge
            if (posX - cubeRadiusX <= 0) {
                posX = cubeRadiusX;
            } else {
                posX = WINDOW_WIDTH - cubeRadiusX;
            }
        }
        // if the cube hits the top or bottom edge of the window, reverse the y velocity
        if (posY - cubeRadiusY <= 0 || posY + cubeRadiusY >= WINDOW_HEIGHT) {
            velY = -velY;
            // Adjust position to prevent sticking to the edge
            if (posY - cubeRadiusY <= 0) {
                posY = cubeRadiusY;
            } else {
                posY = WINDOW_HEIGHT - cubeRadiusY;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // set draw color to black
        SDL_RenderClear(renderer); // clear the screen with black


        // set draw color to white for the cube
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

        updateLineColor();
        SDL_SetRenderDrawColor(renderer, getCurrentLineColor().r, getCurrentLineColor().g, getCurrentLineColor().b, getCurrentLineColor().a); // Use the updated line color

        // scale, rotate, and project each vertex
        Point3D projectedPoints[8];
        for (int i = 0; i < 8; ++i) {

            Point3D point = cubeVertices[i];
            scalePoint(&point); // apply scaling
            rotateX(&point, angleX);
            rotateY(&point, angleY);
            int tempX, tempY; // temporary variables to hold projected values
            project(&point, &tempX, &tempY, posX, posY);
            projectedPoints[i].x = tempX;
            projectedPoints[i].y = tempY;
        }

        // draw each edge of the cube
        for (int i = 0; i < 12; ++i) {
            int startIndex = cubeEdges[i][0];
            int endIndex = cubeEdges[i][1];
            Point3D startPoint = projectedPoints[startIndex];
            Point3D endPoint = projectedPoints[endIndex];
            drawThickLine(renderer, startPoint.x, startPoint.y, endPoint.x, endPoint.y, lineThickness);
        }

        // instructions text
        const char* instructions =
            "I: increase line thickness\n"
            "O: decrease line thickness\n"
            "L: random new color\n"
            "J: increase speed of tumbling\n"
            "K: decrease speed of tumbling\n"
            "N: increase cube size\n"
            "M: decrease sube size\n"
            "P: toggle color changing mode\n"
            "Q: quit";
        // calculate position (top right corner)
        int textX = 50;
        int textY = 50;
        renderText(renderer, font, instructions, textX, textY);

        SDL_RenderPresent(renderer); // update the screen with any rendering performed since the previous call

        // update cube rotation
        angleX += 0.01;
        angleY += 0.01;

        SDL_Delay(16); // delay to cap frame rate
    }
    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer); // clean up renderer
    SDL_DestroyWindow(window); // clean up window
    SDL_Quit(); // clean up SDL
    return 0;
}