#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>

void generateRandomLightColor();
SDL_Color getCurrentLineColor();
void setCurrentLineColor(SDL_Color color);
int getColorChangeMode();
void toggleColorChangeMode();
void updateLineColor();
SDL_Color generateRandomTargetColor();

#endif