#include "utils.h"
#include <stdlib.h>

// global variable for line color
SDL_Color currentLineColor = {255, 255, 255, SDL_ALPHA_OPAQUE}; // start with white
SDL_Color targetLineColor = {255, 255, 255, SDL_ALPHA_OPAQUE}; // target color for smooth transition
int colorChangeMode = 0; // 0 for no color change, 1 for smooth color change

// helper function to generate a random light color
SDL_Color generateRandomTargetColor() {
    SDL_Color color;
    int minColorValue = 100;
    color.r = minColorValue + rand() % (256 - minColorValue);
    color.g = minColorValue + rand() % (256 - minColorValue);
    color.b = minColorValue + rand() % (256 - minColorValue);
    color.a = SDL_ALPHA_OPAQUE;
    return color;
}

// generate a random light color
void generateRandomLightColor() {
    // ensure the color is relatively light by setting a minimum value for each component
    int minColorValue = 100; // adjust this value to make colors lighter or darker
    currentLineColor.r = minColorValue + rand() % (256 - minColorValue);
    currentLineColor.g = minColorValue + rand() % (256 - minColorValue);
    currentLineColor.b = minColorValue + rand() % (256 - minColorValue);
}

// update the current line color smoothly towards the target color
void updateLineColor() {
    if (colorChangeMode == 1) {
        // check if current color matches target color, generate a new target if so
        if (currentLineColor.r == targetLineColor.r && currentLineColor.g == targetLineColor.g && currentLineColor.b == targetLineColor.b) {
            targetLineColor = generateRandomTargetColor();
        }

        // smoothly transition current color towards target color
        currentLineColor.r += (currentLineColor.r < targetLineColor.r) ? 1 : (currentLineColor.r > targetLineColor.r) ? -1 : 0;
        currentLineColor.g += (currentLineColor.g < targetLineColor.g) ? 1 : (currentLineColor.g > targetLineColor.g) ? -1 : 0;
        currentLineColor.b += (currentLineColor.b < targetLineColor.b) ? 1 : (currentLineColor.b > targetLineColor.b) ? -1 : 0;
    }
}

// toggle the color change mode
void toggleColorChangeMode() {
    colorChangeMode = !colorChangeMode;
    if (colorChangeMode == 1) {
        // initialize target color to start transition
        targetLineColor = generateRandomTargetColor();
    }
}

// getter function for colorChangeMode
int getColorChangeMode() {
    // returns the current color change mode
    return colorChangeMode;
}

// function to set the current line color to a specified value
void setCurrentLineColor(SDL_Color color) {
    // set the global variable currentLineColor to the new color
    currentLineColor = color;
}

// function to get the current line color
SDL_Color getCurrentLineColor() {
    // returns the global variable currentLineColor
    return currentLineColor;
}

