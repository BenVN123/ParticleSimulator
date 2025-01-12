#include "platform.h"

#include </opt/homebrew/include/SDL2/SDL.h>  // FIX: change to SDL2/SDL.h
#include <stddef.h>

#include "math.h"
#include "particles.h"

int init_platform(SDL_Window **window, SDL_Renderer **renderer,
                  SDL_Texture **texture, uint32_t *buffer, int width,
                  int height, int scale) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("sdl failed to init: %s\n", SDL_GetError());
        return 0;
    }

    *window = SDL_CreateWindow("balls simulator lolz", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, width * scale,
                               height * scale, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("window could not be created: %s\n", SDL_GetError());
        return 0;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        printf("renderer could not be created: %s\n", SDL_GetError());
        return 0;
    }

    *texture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_RGBA8888,
                                 SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!*texture) {
        printf("texture could not be created: %s\n", SDL_GetError());
        return 0;
    }

    SDL_SetTextureBlendMode(*texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND);

    init_gradient(buffer, width, height);

    return 1;
}

void init_gradient(uint32_t *buffer, int width, int height) {
    // set initial buffer to a transparent gradient
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            uint8_t red = (int)((row * 255) / height);
            uint8_t green = 70;
            uint8_t blue = (int)((col * 255) / width);

            // 0xFFFFFF00 sets initial buffer to transparent, or black
            buffer[(row * width) + col] =
                ((red << 24) | (green << 16) | (blue << 8)) & 0xFFFFFF00;
        }
    }
}

void clear_buffer(uint32_t *buffer, int width, int height) {
    // set all pixels to transparent, or black
    for (int i = 0; i < width * height; ++i) {
        buffer[i] &= 0xFFFFFF00;
    }
}

void draw_particle(Particle *particle, uint32_t *buffer, int width,
                   int height) {
    int x_pos = (int)round(particle->pos->x);
    int y_pos = (int)round(particle->pos->y);
    for (int col = x_pos - particle->radius; col <= x_pos + particle->radius;
         ++col) {
        int y_dist = (int)round(
            sqrt(fabs(pow(particle->radius, 2) - pow(col - x_pos, 2))));
        for (int row = y_pos - y_dist; row <= y_pos + y_dist; ++row) {
            int index = (width * row) + col;
            if (index < width * height) {
                // alpha value is changed to make pixels opaque
                buffer[(width * row) + col] |= 0xFF;
            }
        }
    }
}

void draw_multiple_particles(Particle **particles, size_t len, uint32_t *buffer,
                             int width, int height) {
    for (int i = 0; i < len; ++i) {
        draw_particle(particles[i], buffer, width, height);
    }
}

void update_platform(SDL_Renderer *renderer, SDL_Texture *texture,
                     uint32_t *buffer, Particle **particles, size_t len,
                     int width, int height) {
    clear_buffer(buffer, width, height);
    draw_multiple_particles(particles, len, buffer, width, height);

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

int process_event(int *x_mouse, int *y_mouse) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return 1;
        }

        uint32_t mouseState = SDL_GetMouseState(x_mouse, y_mouse);
        if (!(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))) {
            *x_mouse = -1;
            *y_mouse = -1;
        }
    }
    return 0;
}
