#include <SDL.h>
#include <math.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

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

int main(int argc, char* argv[]) {
    SDL_Window* window; // window handle
    SDL_Renderer* renderer; // renderer handle

    SDL_Init(SDL_INIT_VIDEO); // initialize sdl video subsystem

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
            }
        }

        // update cube position
        posX += velX;
        posY += velY;

        // define a buffer zone to reverse direction when cube hits window edge
        const float bufferZone = 100;
        if (posX < bufferZone || posX > WINDOW_WIDTH - bufferZone) velX = -velX;
        if (posY < bufferZone || posY > WINDOW_HEIGHT - bufferZone) velY = -velY;

        // update rotation angles
        angleX += 0.01;
        angleY += 0.01;

        // set draw color to black and clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // draw each edge of the cube
        for (int i = 0; i < 12; ++i) {
            // get the vertices for the edge
            Point3D p1 = cubeVertices[cubeEdges[i][0]];
            Point3D p2 = cubeVertices[cubeEdges[i][1]];

            // rotate points
            rotateX(&p1, angleX);
            rotateY(&p1, angleY);
            rotateX(&p2, angleX);
            rotateY(&p2, angleY);

            // project 3d points to 2d
            int x1, y1, x2, y2;
            project(&p1, &x1, &y1, posX, posY);
            project(&p2, &x2, &y2, posX, posY);

            // set color for each edge
            switch (i) {
                case 0: SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); break;
                case 1: SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); break;
                case 2: SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); break;
                case 3: SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); break;
                case 4: SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); break;
                case 5: SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); break;
                case 6: SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); break;
                case 7: SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255); break;
                case 8: SDL_SetRenderDrawColor(renderer, 255, 192, 203, 255); break;
                case 9: SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); break;
                case 10: SDL_SetRenderDrawColor(renderer, 128, 0, 0, 255); break;
                case 11: SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255); break;
                default: SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); break;
            }

            // draw the line between projected points
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }

        // present the rendered frame
        SDL_RenderPresent(renderer);

        // delay to control frame rate
        SDL_Delay(16);
    }

    // cleanup sdl resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}