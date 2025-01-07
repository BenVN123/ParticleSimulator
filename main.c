#include "main.h"

#include </opt/homebrew/include/SDL2/SDL.h>  // FIX: change to SDL2/SDL.h
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "particles.h"
#include "platform.h"
int main(void) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    int width = 1500;
    int height = 1000;
    int scale = 1;
    uint32_t *buffer = malloc(sizeof(uint32_t) * width * height);

    float dt = 0.0001;
    int radius = 5;
    size_t p_count = 0;
    size_t p_limit = 100;
    Particle **particles = malloc(sizeof(Particle *) * p_limit);

    if (!init_platform(&window, &renderer, &texture, buffer, width, height,
                       scale)) {
        return -1;
    }

    int x_mouse = -1;
    int y_mouse = -1;
    clock_t prev_time;
    clock_t curr_time;
    float elapsed_time;
    while (!process_event(&x_mouse, &y_mouse)) {
        prev_time = clock();

        if (x_mouse != -1) {
            generate_particle(&particles, &p_count, &p_limit, x_mouse, y_mouse,
                              radius);
            x_mouse = -1;
            y_mouse = -1;
        }
        update_multiple_particles(particles, p_count, dt);
        update_platform(renderer, texture, buffer, particles, p_count, width,
                        height);

        curr_time = clock();
        elapsed_time = (float)(curr_time - prev_time) / CLOCKS_PER_SEC;
        if (elapsed_time < dt) {
            usleep((dt - elapsed_time) * 1000000);
        }
    }
    close_platform(window, renderer, texture);

    free(buffer);
    return 0;
}
