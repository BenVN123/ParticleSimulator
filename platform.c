#include "platform.h"

#include </opt/homebrew/include/SDL2/SDL.h>  // FIX: change to SDL2/SDL.h
#include <stddef.h>

#include "math.h"
#include "physics.h"

int init_platform(SDL_Window *window, SDL_Renderer *renderer,
                  SDL_Texture *texture, uint32_t *buffer, int width, int height,
                  int scale) {
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

    init_gradient(buffer, width, height);

    return 1;
}

void init_gradient(uint32_t *buffer, int width, int height) {
    // FIX: black screen instead of gradient
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            uint8_t red = (int)((row * 255) / height);
            uint8_t green = 128;
            uint8_t blue = (int)((col * 255) / width);

            buffer[(row * width) + col] =
                (red << 24) | (green << 16) | (blue << 8) | 0xFF;
        }
    }
}

void clear_buffer(uint32_t *buffer, int width, int height) {
    // set all pixels to transparent
    for (int i = 0; i < width * height; ++i) {
        buffer[i] &= 0xFFFFFF00;
    }
}

void draw_particle(Particle *particle, int width, uint32_t *buffer) {
    // all rgb values are already preset in buffer (gradient)
    // only alpha value is changed
    for (int col = particle->x - particle->radius;
         col <= particle->x + particle->radius; ++col) {
        int y_dist =
            (int)round(sqrt(pow(particle->radius, 2) - pow(particle->x, 2)));
        for (int row = particle->y - y_dist; row <= particle->y + y_dist;
             ++row) {
            buffer[(width * row) + col] |= 0xFF;
        }
    }
}

void update_platform(SDL_Renderer *renderer, SDL_Texture *texture,
                     uint32_t *buffer, int width) {
    //    clear_buffer(buffer, width, height);
    //    FIX: remove comment when gradient is fixed
    SDL_UpdateTexture(texture, NULL, buffer, width * sizeof(uint32_t));
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

int process_event(void) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return 1;
        }
    }
    return 0;
}
