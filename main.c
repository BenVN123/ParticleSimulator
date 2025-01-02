#include "main.h"

#include </opt/homebrew/include/SDL2/SDL.h>  // FIX: change to SDL2/SDL.h
#include <stdio.h>
#include <unistd.h>

#include "platform.h"

int main(void) {
    SDL_Window *sdl_window = NULL;
    SDL_Surface *sdl_surface = NULL;

    init_platform(sdl_window, sdl_surface, 100, 50);
    sleep(5);
    close_platform(sdl_window, sdl_surface);
}
