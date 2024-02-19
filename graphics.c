#include "graphics.h"
#include <math.h>
#include "utils.h"
#include "types.h"

// draw a thick line with the current color
void drawThickLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int thickness) {
    SDL_Color currentLineColor = getCurrentLineColor();
    SDL_SetRenderDrawColor(renderer, currentLineColor.r, currentLineColor.g, currentLineColor.b, currentLineColor.a);
    for (int i = -thickness / 2; i <= thickness / 2; ++i) {
        for (int j = -thickness / 2; j <= thickness / 2; ++j) {
            SDL_RenderDrawLine(renderer, x1 + i, y1 + j, x2 + i, y2 + j);
        }
    }
}

// projects a 3D point to 2D space
void project(Point3D *point, int *x, int *y, float posX, float posY) {
    *x = (int)(posX + point->x);
    *y = (int)(posY - point->y);
}
