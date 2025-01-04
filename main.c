#include "main.h"

#include </opt/homebrew/include/SDL2/SDL.h>  // FIX: change to SDL2/SDL.h
#include <stdio.h>
#include <unistd.h>

#include "platform.h"

int main(void) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;

    init_platform(window, renderer, texture, 400, 300, 2);
    sleep(5);
    close_platform(window, renderer, texture);
}
