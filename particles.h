#ifndef __PARTICLES__
#define __PARTICLES__

#include <stddef.h>

#define GRAVITY 1000
#define COLLISION_LOSS_RATIO 0.95
#define DRAG_LOSS_RATIO 0.9999

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

void update_particle(Particle *particle, long double dt, int width, int height);
void update_multiple_particles(Particle **particles, size_t count,
                               long double dt, int width, int height);
void generate_particle(Particle ***particles, size_t *count, size_t *limit,
                       int x, int y, int radius);

int check_collision(Particle *p1, Particle *p2);
void handle_particle_collision(Particle *p1, Particle *p2);
void handle_x_border_collision(Particle *p, int width);
void handle_y_border_collision(Particle *p, int height);

#endif
