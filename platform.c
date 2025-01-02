#include </opt/homebrew/include/SDL2/SDL.h>  // FIX: change to SDL2/SDL.h
#include <stddef.h>

int init_platform(SDL_Window *window, SDL_Surface *surface, int width,
                  int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        perror("sdl failed to init\n");
        return 0;
    }

    window = SDL_CreateWindow("balls simulator lolz", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, width, height,
                              SDL_WINDOW_SHOWN);
    if (window == NULL) {
        perror("sdl window could not be created\n");
        return 0;
    }

    surface = SDL_GetWindowSurface(window);
    return 1;
}

void close_platform(SDL_Window *window, SDL_Surface *surface) {
    SDL_FreeSurface(surface);
    surface = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}
