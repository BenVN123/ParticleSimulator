#ifndef __PLATFORM__
#define __PLATFORM__

#include </opt/homebrew/include/SDL2/SDL.h>  // FIX: change to SDL2/SDL.h

int init_platform(SDL_Window *window, SDL_Surface *surface, int width,
                  int height);
void close_platform(SDL_Window *window, SDL_Surface *surface);

#endif
