#ifndef TEXT_H
#define TEXT_H

#include <SDL.h>
#include <SDL_ttf.h>

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y);

#endif