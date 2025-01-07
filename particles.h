#ifndef __PARTICLES__
#define __PARTICLES__

#define GRAVITY 9.8

#include <stddef.h>

typedef struct Vector {
    float x;
    float y;
} Vector;

typedef struct Particle {
    int radius;
    Vector *curr_pos;
    Vector *prev_pos;
    Vector *vel;
    Vector *accel;
} Particle;

void update_particle(Particle *particle, float dt);
void update_multiple_particles(Particle **particles, size_t count, float dt);
void generate_particle(Particle ***particles, size_t *count, size_t *limit,
                       int x, int y, int radius);

#endif
