#include <SDL.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <SDL_ttf.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// global variable for line color
SDL_Color currentLineColor = {255, 255, 255, SDL_ALPHA_OPAQUE}; // start with white

// generate a random light color
void generateRandomLightColor() {
    // ensure the color is relatively light by setting a minimum value for each component
    int minColorValue = 100; // adjust this value to make colors lighter or darker
    currentLineColor.r = minColorValue + rand() % (256 - minColorValue);
    currentLineColor.g = minColorValue + rand() % (256 - minColorValue);
    currentLineColor.b = minColorValue + rand() % (256 - minColorValue);
}

// defines the structure for 3D points
typedef struct {
    float x, y, z; // coordinates
} Point3D;

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

float angleX = 0.0, angleY = 0.0; // rotation angles
float posX = WINDOW_WIDTH / 2, posY = WINDOW_HEIGHT / 2; // starting position
float velX = 2.0, velY = 2.0; // movement speed
int lineThickness = 1; // initial line thickness

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

// projects a 3D point to 2D space
void project(Point3D *point, int *x, int *y, float posX, float posY) {
    *x = (int)(posX + point->x);
    *y = (int)(posY - point->y);
}

// draw a thick line with the current color
void drawThickLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int thickness) {
    SDL_SetRenderDrawColor(renderer, currentLineColor.r, currentLineColor.g, currentLineColor.b, currentLineColor.a);
    for (int i = -thickness / 2; i <= thickness / 2; ++i) {
        for (int j = -thickness / 2; j <= thickness / 2; ++j) {
            SDL_RenderDrawLine(renderer, x1 + i, y1 + j, x2 + i, y2 + j);
        }
    }
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y) {
    SDL_Color white = {255, 255, 255}; // text color
    int lineHeight = TTF_FontLineSkip(font); // get the recommended line spacing for the font

    char buffer[256]; // buffer to hold each line of text
    const char* start = text;
    const char* end = NULL;
    while ((end = strchr(start, '\n')) != NULL) { // find the next newline character
        size_t len = end - start;
        if (len >= sizeof(buffer)) len = sizeof(buffer) - 1; // ensure buffer is not overflowed
        strncpy(buffer, start, len); // copy the line into the buffer
        buffer[len] = '\0'; // null-terminate the string

        SDL_Surface* surface = TTF_RenderText_Solid(font, buffer, white);
        if (!surface) {
            printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
            return;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
            SDL_FreeSurface(surface); // clean up the surface if texture creation failed
            return;
        }

        SDL_Rect textRect = {x, y, surface->w, surface->h};
        SDL_FreeSurface(surface);

        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_DestroyTexture(texture); // clean up

        y += lineHeight; // move to the next line
        start = end + 1; // skip the newline character
    }

    // Render the last line if there's no newline at the end of the text
    if (*start) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, start, white);
        if (!surface) {
            printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
            return;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
            SDL_FreeSurface(surface); // clean up the surface if texture creation failed
            return;
        }

        SDL_Rect textRect = {x, y, surface->w, surface->h};
        SDL_FreeSurface(surface);

        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_DestroyTexture(texture); // clean up
    }
}

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
    TTF_Font* font = TTF_OpenFont("./terminal-grotesque.ttf", 24);
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
            }
        }

        // update cube position based on velocity
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

        // rotate and project each vertex
        Point3D projectedPoints[8];
        for (int i = 0; i < 8; ++i) {
            Point3D point = cubeVertices[i];
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
            "i: increase line thickness\n"
            "o: decrease line thickness\n"
            "l: random new color\n"
            "q: quit";
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