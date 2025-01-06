#include "main.h"

#include </opt/homebrew/include/SDL2/SDL.h>  // FIX: change to SDL2/SDL.h
#include <stdio.h>
#include <unistd.h>

#include "platform.h"

int main(void) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    int width = 1500;
    int height = 1000;
    int scale = 1;
    uint32_t *buffer = malloc(sizeof(uint32_t) * width * height);
    size_t particle_count = 100;
    Particle **particles = malloc(sizeof(Particle *) * particle_count);

    if (!init_platform(&window, &renderer, &texture, buffer, width, height,
                       scale)) {
        return -1;
    }
    while (!process_event()) {
        update_platform(renderer, texture, buffer, particles, particle_count,
                        width, height);
    }
    close_platform(window, renderer, texture);

    free(buffer);
    return 0;
}
