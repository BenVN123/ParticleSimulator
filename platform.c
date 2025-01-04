#include "platform.h"

#include </opt/homebrew/include/SDL2/SDL.h>  // FIX: change to SDL2/SDL.h
#include <stddef.h>

#include "physics.h"

int init_platform(SDL_Window *window, SDL_Renderer *renderer,
                  SDL_Texture *texture, int width, int height, int scale) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("sdl failed to init: %s\n", SDL_GetError());
        return 0;
    }

    window = SDL_CreateWindow("balls simulator lolz", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, width * scale,
                              height * scale, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("window could not be created: %s\n", SDL_GetError());
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("renderer could not be created: %s\n", SDL_GetError());
        return 0;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture) {
        printf("texture could not be created: %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}

void clear_buffer(uint32_t *buffer, size_t len) {
    for (int i = 0; i < len; ++i) {
        buffer[i] &= 0xFFFFFF00;
    }
}

void draw_particle(Particle *particle, int width, int height,
                   uint32_t *buffer) {
    /*
     * for pixel in particle:
     *
     *
     */

    pixel = ((pixel >> 0xFF) << 0xFF) | 0xFF;
}

void update_platform(SDL_Renderer *renderer, SDL_Texture *texture,
                     uint32_t const *buffer, int pitch) {
    SDL_UpdateTexture(texture, NULL, buffer, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void close_platform(SDL_Window *window, SDL_Renderer *renderer,
                    SDL_Texture *texture) {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    texture = NULL;
    renderer = NULL;
    window = NULL;
}
