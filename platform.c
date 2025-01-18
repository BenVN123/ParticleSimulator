#include "platform.h"

#include </opt/homebrew/include/SDL2/SDL.h>  // FIX: change to SDL2/SDL.h
#include <math.h>
#include <stddef.h>

#include "particles.h"
#include "utils.h"

int init_platform(Simulator *sim) {
    sim->window = NULL;
    sim->renderer = NULL;
    sim->texture = NULL;
    sim->buffer = malloc(sizeof(uint32_t) * sim->width * sim->height);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("sdl failed to init: %s\n", SDL_GetError());
        return 0;
    }

    sim->window =
        SDL_CreateWindow("balls simulator lolz", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, sim->width * sim->scale,
                         sim->height * sim->scale, SDL_WINDOW_SHOWN);
    if (sim->window == NULL) {
        printf("window could not be created: %s\n", SDL_GetError());
        return 0;
    }

    sim->renderer =
        SDL_CreateRenderer(sim->window, -1, SDL_RENDERER_ACCELERATED);
    if (!sim->renderer) {
        printf("renderer could not be created: %s\n", SDL_GetError());
        return 0;
    }

    sim->texture =
        SDL_CreateTexture(sim->renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_STREAMING, sim->width, sim->height);
    if (!sim->texture) {
        printf("texture could not be created: %s\n", SDL_GetError());
        return 0;
    }

    SDL_SetTextureBlendMode(sim->texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(sim->renderer, SDL_BLENDMODE_BLEND);

    // set initial buffer to a transparent gradient
    for (int row = 0; row < sim->height; ++row) {
        for (int col = 0; col < sim->width; ++col) {
            uint8_t red = (int)((row * 255) / sim->height);
            uint8_t green = 70;
            uint8_t blue = (int)((col * 255) / sim->width);

            // 0xFFFFFF00 sets initial buffer to transparent, or black
            sim->buffer[(row * sim->width) + col] =
                ((red << 24) | (green << 16) | (blue << 8)) & 0xFFFFFF00;
        }
    }

    return 1;
}

void clear_buffer(Simulator *sim) {
    // set all pixels to transparent, or black
    for (int i = 0; i < sim->width * sim->height; ++i) {
        sim->buffer[i] &= 0xFFFFFF00;
    }
}

// FIX: heap overflow here......

void draw_particle(Simulator *sim, Particle *particle) {
    int x_pos = (int)round(particle->pos->x);
    int y_pos = (int)round(particle->pos->y);
    for (int col = x_pos - particle->radius; col <= x_pos + particle->radius;
         ++col) {
        int y_dist = (int)round(
            sqrt(fabs(pow(particle->radius, 2) - pow(col - x_pos, 2))));
        for (int row = y_pos - y_dist; row <= y_pos + y_dist; ++row) {
            int index = (sim->width * row) + col;
            if (index < sim->width * sim->height) {
                // alpha value is changed to make pixels opaque
                sim->buffer[(sim->width * row) + col] |= 0xFF;
            }
        }
    }
}

void draw_multiple_particles(Simulator *sim) {
    for (int i = 0; i < sim->p_count; ++i) {
        draw_particle(sim, sim->particles[i]);
    }
}

void update_platform(Simulator *sim) {
    clear_buffer(sim);
    draw_multiple_particles(sim);

    SDL_UpdateTexture(sim->texture, NULL, sim->buffer,
                      sim->width * sizeof(uint32_t));
    SDL_RenderClear(sim->renderer);
    SDL_RenderCopy(sim->renderer, sim->texture, NULL, NULL);
    SDL_RenderPresent(sim->renderer);
}

void close_platform(Simulator *sim) {
    SDL_DestroyTexture(sim->texture);
    SDL_DestroyRenderer(sim->renderer);
    SDL_DestroyWindow(sim->window);
    SDL_Quit();

    sim->texture = NULL;
    sim->renderer = NULL;
    sim->window = NULL;
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
