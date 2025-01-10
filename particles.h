#ifndef __PARTICLES__
#define __PARTICLES__

#include <stddef.h>

#define GRAVITY 100
#define LOSS_RATIO 0.95

typedef struct Vector {
    long double x;
    long double y;
} Vector;

typedef struct Particle {
    int radius;
    volatile Vector *curr_pos;
    volatile Vector *prev_pos;
    volatile Vector *vel;
    volatile Vector *accel;
} Particle;

void update_particle(Particle *particle, long double dt);
void update_multiple_particles(Particle **particles, size_t count,
                               long double dt);
void generate_particle(Particle ***particles, size_t *count, size_t *limit,
                       int x, int y, int radius);

int check_collision(Particle *p1, Particle *p2);
void handle_particle_collision(Particle *p1, Particle *p2);
void handle_x_border_collision(Particle *p);
void handle_y_border_collision(Particle *p);

#endif
