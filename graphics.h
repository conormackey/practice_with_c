#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include "types.h"

void drawThickLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int thickness);
void project(Point3D *point, int *x, int *y, float posX, float posY);

#endif