#include "main.h"

#include </opt/homebrew/include/SDL2/SDL.h>  // FIX: change to SDL2/SDL.h
#include <time.h>
#include <unistd.h>

#include "particles.h"
#include "platform.h"
#include "segments.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    // WARN: consider limiting the areas allowed for particle generation to
    // prevent edge cases and cut-off balls

    if (argc < 5) {
        printf(
            "you did it wrong. use this format:\n\t./heheballs [width (int)] "
            "[height (int)] [scale (int)] [fps (int)]\n");
        return -1;
    }

    Simulator *sim = malloc(sizeof(Simulator));

    sim->width = atoi(argv[1]);
    sim->height = atoi(argv[2]);
    sim->scale = atoi(argv[3]);
    sim->fps = atoi(argv[4]);

    if (!init_platform(sim)) {
        return -1;
    }

    int radius = 5;
    sim->p_count = 0;
    sim->p_limit = 100;
    sim->particles = malloc(sizeof(Particle *) * sim->p_limit);
    sim->dt = 1. / sim->fps;

    sim->segments = generate_segments(sim);

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
            generate_particle(sim, x_mouse, y_mouse, radius);
            x_mouse = -1;
            y_mouse = -1;
        }

        all_segment_checks(sim);
        update_multiple_particles(sim);
        update_platform(sim);

        curr_time = clock();
        elapsed_time = ((long double)(curr_time - prev_time)) / CLOCKS_PER_SEC;
        if (elapsed_time < sim->dt) {
            usleep((sim->dt - elapsed_time) * 1000000);
        } else {
            printf("Slow loop on iteration %d: %Lfs\n", iter,
                   elapsed_time - sim->dt);
        }

        ++iter;
    }

    close_platform(sim);
    return 0;
}
