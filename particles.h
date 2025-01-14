#ifndef __PARTICLES__
#define __PARTICLES__

#include <stddef.h>

#include "utils.h"

#define GRAVITY 1000
#define COLLISION_LOSS_RATIO 0.75

#define MIN_RADIUS 50
#define MAX_RADIUS 400

void update_particle(Particle *particle, Segment **segments, long double dt,
                     int width, int height);
void update_multiple_particles(Particle **particles, Segment **segments,
                               size_t count, long double dt, int width,
                               int height);
void generate_particle(Particle ***particles, size_t *count, size_t *limit,
                       int x, int y, int radius);

int check_collision(Particle *p1, Particle *p2);
void handle_particle_collision(Particle *p1, Particle *p2);
void handle_x_border_collision(Particle *p, int width);
void handle_y_border_collision(Particle *p, int height);

#endif
