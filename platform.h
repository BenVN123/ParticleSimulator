#ifndef __PLATFORM__
#define __PLATFORM__

#include </opt/homebrew/include/SDL2/SDL.h>  // FIX: change to SDL2/SDL.h

#include "physics.h"

int init_platform(SDL_Window *window, SDL_Renderer *renderer,
                  SDL_Texture *texture, uint32_t *buffer, int width, int height,
                  int scale);
void init_gradient(uint32_t *buffer, int width, int height);
void clear_buffer(uint32_t *buffer, int width, int height);
void draw_particle(Particle *particle, int width, uint32_t *buffer);
void update_platform(SDL_Renderer *renderer, SDL_Texture *texture,
                     uint32_t *buffer, int width);
void close_platform(SDL_Window *window, SDL_Renderer *renderer,
                    SDL_Texture *texture);
int process_event(void);

#endif
