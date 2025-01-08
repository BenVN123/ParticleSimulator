#include "main.h"

#include </opt/homebrew/include/SDL2/SDL.h>  // FIX: change to SDL2/SDL.h
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "particles.h"
#include "platform.h"
int main(void) {
    // TODO: user input for width, height, scale, radius, and fps

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    int width = 1500;
    int height = 1000;
    int scale = 1;
    uint32_t *buffer = malloc(sizeof(uint32_t) * width * height);

    int radius = 5;
    size_t p_count = 0;
    size_t p_limit = 100;
    Particle **particles = malloc(sizeof(Particle *) * p_limit);

    // PERF: current code cannot handle 240 fps, lags slow each loop
    long double dt = 0.016667;  // 60 fps
    // long double dt = 0.008333;  // 120 fps;
    // long double dt = 0.004167;  // 240 fps;

    if (!init_platform(&window, &renderer, &texture, buffer, width, height,
                       scale)) {
        return -1;
    }

    int x_mouse = -1;
    int y_mouse = -1;
    clock_t prev_time;
    clock_t curr_time;
    long double elapsed_time;
    while (!process_event(&x_mouse, &y_mouse)) {
        // HACK: particles generate on top of each other. decrease allowed rate
        // of generation or randomize the exact point of generation
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
        elapsed_time = ((long double)(curr_time - prev_time)) / CLOCKS_PER_SEC;
        if (elapsed_time < dt) {
            usleep((dt - elapsed_time) * 1000000);
        } else {
            printf("Slow loop: %Lfs\n", elapsed_time - dt);
        }
    }
    close_platform(window, renderer, texture);

    free(buffer);
    return 0;
}
