#include "particles.h"

#include <math.h>
#include <stddef.h>
#include <stdlib.h>

void update_particle(Particle *particle, float dt) {
    int temp_x = particle->curr_pos->x;
    int temp_y = particle->curr_pos->y;
    particle->curr_pos->x =
        (int)round((particle->curr_pos->x * 2) - particle->prev_pos->x +
                   (particle->accel->x * dt * dt));
    particle->curr_pos->y =
        (int)round((particle->curr_pos->y * 2) - particle->prev_pos->y +
                   (particle->accel->y * dt * dt));
    particle->prev_pos->x = temp_x;
    particle->prev_pos->y = temp_y;

    particle->vel->x += particle->accel->x * dt;
    particle->vel->y += particle->accel->y * dt;

    particle->accel->x = 0;
    particle->accel->y = GRAVITY;
}

void update_multiple_particles(Particle **particles, size_t count, float dt) {
    for (int i = 0; i < count; ++i) {
        update_particle(particles[i], dt);
    }
}

void generate_particle(Particle ***particles, size_t *count, size_t *limit,
                       int x, int y, int radius) {
    // FIX: reallocation bugs

    if (*count == *limit) {
        *limit = (size_t)*limit * 1.5;
        *particles = realloc(particles, sizeof(Particle *) * (*limit));
    }

    (*particles)[*count] = malloc(sizeof(Particle));
    (*particles)[*count]->radius = radius;

    (*particles)[*count]->curr_pos = malloc(sizeof(Vector));
    (*particles)[*count]->curr_pos->x = x;
    (*particles)[*count]->curr_pos->y = y;

    (*particles)[*count]->prev_pos = malloc(sizeof(Vector));
    (*particles)[*count]->prev_pos->x = x;
    (*particles)[*count]->prev_pos->y = y;

    (*particles)[*count]->vel = malloc(sizeof(Vector));
    (*particles)[*count]->vel->x = 0;
    (*particles)[*count]->vel->y = 0;

    (*particles)[*count]->accel = malloc(sizeof(Vector));
    (*particles)[*count]->accel->x = 0;
    (*particles)[*count]->accel->y = GRAVITY;

    ++(*count);
}
