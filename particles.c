#include "particles.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: detect and handle collisions

void update_particle(Particle *particle, long double dt) {
    particle->vel->x += particle->accel->x * dt;
    particle->vel->y += particle->accel->y * dt;

    particle->curr_pos->x += particle->vel->x * dt;
    particle->curr_pos->y += particle->vel->y * dt;

    particle->accel->x = 0;
    particle->accel->y = GRAVITY;
}

void update_multiple_particles(Particle **particles, size_t count,
                               long double dt) {
    for (int i = 0; i < count; ++i) {
        update_particle(particles[i], dt);
    }
}

void generate_particle(Particle ***particles, size_t *count, size_t *limit,
                       int x, int y, int radius) {
    if (*count == *limit) {
        *limit = (size_t)*limit * 1.5;
        *particles = realloc(*particles, sizeof(Particle *) * (*limit));
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
