#include "particles.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "segments.h"
#include "utils.h"

// TODO: implement verlet integration for more accurate simulation

void update_particle(Simulator *sim, Particle *particle) {
    particle->vel->x += particle->accel->x * sim->dt;
    particle->vel->y += particle->accel->y * sim->dt;

    int old_x = particle->pos->x;
    int old_y = particle->pos->y;

    particle->pos->x += particle->vel->x * sim->dt;
    particle->pos->y += particle->vel->y * sim->dt;

    particle->accel->x = 0;
    particle->accel->y = GRAVITY;

    handle_x_border_collision(particle, sim->width);
    handle_y_border_collision(particle, sim->height);

    update_particle_segment(sim, particle, old_x, old_y);
}

void update_multiple_particles(Simulator *sim) {
    for (int i = 0; i < sim->p_count; ++i) {
        update_particle(sim, sim->particles[i]);
    }
}

void generate_particle(Simulator *sim, int x, int y, int radius) {
    if (sim->p_count == sim->p_limit) {
        sim->p_limit *= 1.5;
        sim->particles =
            realloc(sim->particles, sizeof(Particle *) * (sim->p_limit));
    }

    Particle *p = malloc(sizeof(Particle));

    p->radius = radius;

    p->pos = malloc(sizeof(Vector));
    p->pos->x = x;
    p->pos->y = y;

    p->vel = malloc(sizeof(Vector));
    p->vel->x = 0;
    p->vel->y = 0;

    p->accel = malloc(sizeof(Vector));
    p->accel->x = 0;
    p->accel->y = GRAVITY;

    sim->particles[sim->p_count] = p;
    ++sim->p_count;

    Segment *s = get_segment(sim, x, y);
    add_segment_particle(s, p);
}

int check_collision(Particle *p1, Particle *p2) {
    if (p1->radius + p2->radius > sqrt(pow(p1->pos->x - p2->pos->x, 2) +
                                       pow(p1->pos->y - p2->pos->y, 2))) {
        return 1;
    }

    return 0;
}

// FIX: collision physics is wrong 😾😾😾😾

void handle_particle_collision(Particle *p1, Particle *p2) {
    if (check_collision(p1, p2)) {
        long double p1_mass = calculate_mass(p1);
        long double p2_mass = calculate_mass(p2);

        long double x1 = p1_mass * p1->vel->x + p2_mass * p2->vel->x;
        long double x2 = p1->vel->x - p2->vel->x;
        p2->vel->x = ((x1 + (p1_mass * x2)) / (p1_mass + p2_mass)) *
                     COLLISION_LOSS_RATIO;
        p1->vel->x = (p2->vel->x - x2) * COLLISION_LOSS_RATIO;

        long double y1 = p1_mass * p1->vel->y + p2_mass * p2->vel->y;
        long double y2 = p1->vel->y - p2->vel->y;
        p2->vel->y = ((y1 + (p1_mass * y2)) / (p1_mass + p2_mass)) *
                     COLLISION_LOSS_RATIO;
        p1->vel->y = (p2->vel->y - y2) * COLLISION_LOSS_RATIO;
    }
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
