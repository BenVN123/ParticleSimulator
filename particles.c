#include "particles.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

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

// TODO: implement collisions

int check_collision(Particle *p1, Particle *p2) {
    if (p1->radius + p2->radius >
        sqrt(pow(p1->curr_pos->x - p2->curr_pos->x, 2) +
             pow(p1->curr_pos->y - p2->curr_pos->y, 2))) {
        return 1;
    }

    return 0;
}

void handle_particle_collision(Particle *p1, Particle *p2) {
    long double p1_mass = calculate_mass(p1);
    long double p2_mass = calculate_mass(p1);

    long double x1 = p1_mass * p1->vel->x + p2_mass * p2->vel->x;
    long double x2 = p1->vel->x - p2->vel->x;
    p2->vel->x = ((x1 + (p1_mass * x2)) / (p1_mass + p2_mass)) * LOSS_RATIO;
    p1->vel->x = (p2->vel->x - x2) * LOSS_RATIO;

    long double y1 = p1_mass * p1->vel->y + p2_mass * p2->vel->y;
    long double y2 = p1->vel->y - p2->vel->y;
    p2->vel->y = ((y1 + (p1_mass * y2)) / (p1_mass + p2_mass)) * LOSS_RATIO;
    p1->vel->y = (p2->vel->y - y2) * LOSS_RATIO;
}

void handle_x_border_collision(Particle *p) { p->vel->x *= -LOSS_RATIO; }

void handle_y_border_collision(Particle *p) { p->vel->y *= -LOSS_RATIO; }
