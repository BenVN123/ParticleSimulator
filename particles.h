#ifndef __PARTICLES__
#define __PARTICLES__

#include <stddef.h>

#include "utils.h"

#define GRAVITY 1000
#define COLLISION_LOSS_RATIO 0.75

#define MIN_RADIUS 20
#define MAX_RADIUS 20

void update_particle(Simulator *sim, Particle *particle);
void update_multiple_particles(Simulator *sim);
void generate_particle(Simulator *sim, int x, int y, int radius);
int check_collision(Particle *p1, Particle *p2);
void handle_particle_collision(Particle *p1, Particle *p2);
void handle_x_border_collision(Particle *p, int width);
void handle_y_border_collision(Particle *p, int height);

#endif
