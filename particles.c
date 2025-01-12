#include "particles.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

// TODO: implement verlet integration for more accurate simulation

void update_particle(Particle *particle, long double dt, int width,
                     int height) {
    particle->vel->x += particle->accel->x * dt;
    particle->vel->y += particle->accel->y * dt;

    particle->pos->x += particle->vel->x * dt;
    particle->pos->y += particle->vel->y * dt;

    particle->accel->x = 0;
    particle->accel->y = GRAVITY;

    handle_x_border_collision(particle, width);
    handle_y_border_collision(particle, height);
}

void update_multiple_particles(Particle **particles, size_t count,
                               long double dt, int width, int height) {
    for (int i = 0; i < count; ++i) {
        update_particle(particles[i], dt, width, height);
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

    (*particles)[*count]->pos = malloc(sizeof(Vector));
    (*particles)[*count]->pos->x = x;
    (*particles)[*count]->pos->y = y;

    (*particles)[*count]->vel = malloc(sizeof(Vector));
    (*particles)[*count]->vel->x = 0;
    (*particles)[*count]->vel->y = 0;

    (*particles)[*count]->accel = malloc(sizeof(Vector));
    (*particles)[*count]->accel->x = 0;
    (*particles)[*count]->accel->y = GRAVITY;

    ++(*count);
}

// TODO: implement particle collisions

int check_collision(Particle *p1, Particle *p2) {
    if (p1->radius + p2->radius > sqrt(pow(p1->pos->x - p2->pos->x, 2) +
                                       pow(p1->pos->y - p2->pos->y, 2))) {
        return 1;
    }

    return 0;
}

void handle_particle_collision(Particle *p1, Particle *p2) {
    long double p1_mass = calculate_mass(p1);
    long double p2_mass = calculate_mass(p1);

    long double x1 = p1_mass * p1->vel->x + p2_mass * p2->vel->x;
    long double x2 = p1->vel->x - p2->vel->x;
    p2->vel->x =
        ((x1 + (p1_mass * x2)) / (p1_mass + p2_mass)) * COLLISION_LOSS_RATIO;
    p1->vel->x = (p2->vel->x - x2) * COLLISION_LOSS_RATIO;

    long double y1 = p1_mass * p1->vel->y + p2_mass * p2->vel->y;
    long double y2 = p1->vel->y - p2->vel->y;
    p2->vel->y =
        ((y1 + (p1_mass * y2)) / (p1_mass + p2_mass)) * COLLISION_LOSS_RATIO;
    p1->vel->y = (p2->vel->y - y2) * COLLISION_LOSS_RATIO;
}

void handle_x_border_collision(Particle *p, int width) {
    if (p->pos->x + p->radius > width || p->pos->x - p->radius < 0) {
        p->vel->x *= -COLLISION_LOSS_RATIO;
    }
}

void handle_y_border_collision(Particle *p, int height) {
    if (p->pos->y + p->radius > height || p->pos->y - p->radius < 0) {
        p->vel->y *= -COLLISION_LOSS_RATIO;
    }

    if (p->pos->y > height - p->radius) {
        p->pos->y = height - p->radius;
    }
}
