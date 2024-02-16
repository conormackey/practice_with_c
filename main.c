#include <SDL.h>
#include <math.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

// Cube structure
typedef struct {
    float x, y, z; // Position
} Point3D;

// Define 8 vertices of a cube
Point3D cubeVertices[8] = {
    {-50, -50, -50}, {50, -50, -50}, {50, 50, -50}, {-50, 50, -50},
    {-50, -50, 50}, {50, -50, 50}, {50, 50, 50}, {-50, 50, 50}
};

// Define cube edges: each pair of numbers are indices into the cubeVertices array
int cubeEdges[12][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0},
    {4, 5}, {5, 6}, {6, 7}, {7, 4},
    {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

float angleX = 0.0, angleY = 0.0; // Rotation angles
float posX = WINDOW_WIDTH / 2, posY = WINDOW_HEIGHT / 2; // Initial position
float velX = 2.0, velY = 2.0; // Velocity

// Function to rotate a point around the X axis
void rotateX(Point3D* point, float angle) {
    float y = point->y;
    point->y = cos(angle) * y - sin(angle) * point->z;
    point->z = sin(angle) * y + cos(angle) * point->z;
}

// Function to rotate a point around the Y axis
void rotateY(Point3D* point, float angle) {
    float x = point->x;
    point->x = cos(angle) * x + sin(angle) * point->z;
    point->z = -sin(angle) * x + cos(angle) * point->z;
}

// Function to project a 3D point to 2D, considering the cube's position
void project(Point3D *point, int *x, int *y, float posX, float posY) {
    // Adjust projection to center the cube in the window correctly
    // and include the cube's current position
    *x = (int)(posX + point->x);
    *y = (int)(posY - point->y);
}

int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;

    // Initialize SDL2
    SDL_Init(SDL_INIT_VIDEO);

    // Create an application window
    window = SDL_CreateWindow("Bouncing Tumbling Cube", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int running = 1;
    SDL_Event event;
    while (running) {
        // Check for events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Update cube position and bounce off edges
        posX += velX;
        posY += velY;

        // Adjust boundary checks to ensure the cube stays within the window's visible area
        // Reduce the buffer zone to allow the cube to move closer to the edges
        const float bufferZone = 100; // Smaller buffer zone for bouncing
        if (posX < bufferZone || posX > WINDOW_WIDTH - bufferZone) velX = -velX;
        if (posY < bufferZone || posY > WINDOW_HEIGHT - bufferZone) velY = -velY;

        // Rotate cube
        angleX += 0.01;
        angleY += 0.01;

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw cube
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
        for (int i = 0; i < 12; ++i) { // Draw each edge
            Point3D p1 = cubeVertices[cubeEdges[i][0]];
            Point3D p2 = cubeVertices[cubeEdges[i][1]];

            // Rotate points
            rotateX(&p1, angleX);
            rotateY(&p1, angleY);
            rotateX(&p2, angleX);
            rotateY(&p2, angleY);

            // Project 3D points to 2D and draw line, considering the cube's position
            int x1, y1, x2, y2;
            project(&p1, &x1, &y1, posX, posY);
            project(&p2, &x2, &y2, posX, posY);
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }

        // Update screen
        SDL_RenderPresent(renderer);

        // Delay to control frame rate
        SDL_Delay(16); // Approximately 60 frames per second
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
