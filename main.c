#include "main.h"

#include </opt/homebrew/include/SDL2/SDL.h>  // FIX: change to SDL2/SDL.h
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "particles.h"
#include "platform.h"
#include "segments.h"

int main(int argc, char *argv[]) {
    // WARN: consider limiting the areas allowed for particle generation to
    // prevent edge cases and cut-off balls

    // NOTE: consider using global variables

    if (argc < 5) {
        printf(
            "you did it wrong. use this format:\n\t./heheballs [width (int)] "
            "[height (int)] [scale (int)] [fps (int)]\n");
        return -1;
    }

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int scale = atoi(argv[3]);
    int fps = atoi(argv[4]);

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    uint32_t *buffer = malloc(sizeof(uint32_t) * width * height);

    if (!init_platform(&window, &renderer, &texture, buffer, width, height,
                       scale)) {
        return -1;
    }

    int radius = 5;
    size_t p_count = 0;
    size_t p_limit = 100;
    Particle **particles = malloc(sizeof(Particle *) * p_limit);
    long double dt = 1. / fps;

    int s_count;
    Segment **segments = generate_segments(width, height, &s_count);

    int x_mouse = -1;
    int y_mouse = -1;
    clock_t prev_time;
    clock_t curr_time;
    long double elapsed_time;
    int iter = 0;
    while (!process_event(&x_mouse, &y_mouse)) {
        // PERF: lags slow each loop as fps becomes larger, making simulation
        // run slow

        // HACK: particles generate on top of each other. decrease allowed rate
        // of generation or randomize the exact point of generation

        // NOTE: consider having particles be generated at random sizes to
        // simulate the collisions of differently-sized particles

        prev_time = clock();

        if (x_mouse != -1) {
            generate_particle(&particles, &p_count, &p_limit, x_mouse, y_mouse,
                              radius);
            x_mouse = -1;
            y_mouse = -1;
        }

        all_segment_checks(segments, s_count, width, height);
        update_multiple_particles(particles, segments, p_count, dt, width,
                                  height);
        update_platform(renderer, texture, buffer, particles, p_count, width,
                        height);

        curr_time = clock();
        elapsed_time = ((long double)(curr_time - prev_time)) / CLOCKS_PER_SEC;
        if (elapsed_time < dt) {
            usleep((dt - elapsed_time) * 1000000);
        } else {
            printf("Slow loop on iteration %d: %Lfs\n", iter,
                   elapsed_time - dt);
        }

        ++iter;
    }
    close_platform(window, renderer, texture);

    free(buffer);
    return 0;
}
