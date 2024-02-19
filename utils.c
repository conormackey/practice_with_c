#include "utils.h"
#include <stdlib.h>

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

// returns the current line color
SDL_Color getCurrentLineColor() {
    return currentLineColor;
}

// sets the current line color
void setCurrentLineColor(SDL_Color color) {
    currentLineColor = color;
}