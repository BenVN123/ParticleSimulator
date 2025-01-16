#ifndef __PLATFORM__
#define __PLATFORM__

#include </opt/homebrew/include/SDL2/SDL.h>  // FIX: change to SDL2/SDL.h

#include "particles.h"
#include "utils.h"

int init_platform(Simulator *sim);
void clear_buffer(Simulator *sim);
void draw_particle(Simulator *sim, Particle *particle);
void draw_multiple_particles(Simulator *sim);
void update_platform(Simulator *sim);
void close_platform(Simulator *sim);
int process_event(int *x_mouse, int *y_mouse);

#endif
