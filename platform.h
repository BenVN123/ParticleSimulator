#ifndef __PLATFORM__
#define __PLATFORM__

#include </opt/homebrew/include/SDL2/SDL.h>  // FIX: change to SDL2/SDL.h

#include "particles.h"

int init_platform(SDL_Window **window, SDL_Renderer **renderer,
                  SDL_Texture **texture, uint32_t *buffer, int width,
                  int height, int scale);
void init_gradient(uint32_t *buffer, int width, int height);
void clear_buffer(uint32_t *buffer, int width, int height);
void draw_particle(Particle *particle, uint32_t *buffer, int width, int height);
void draw_multiple_particles(Particle **particles, size_t len, uint32_t *buffer,
                             int width, int height);
void update_platform(SDL_Renderer *renderer, SDL_Texture *texture,
                     uint32_t *buffer, Particle **particles, size_t len,
                     int width, int height);
void close_platform(SDL_Window *window, SDL_Renderer *renderer,
                    SDL_Texture *texture);
int process_event(int *x_mouse, int *y_mouse);

#endif
