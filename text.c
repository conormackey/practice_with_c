#include "text.h"
#include <string.h>

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