#ifndef __PLATFORM__
#define __PLATFORM__

#include </opt/homebrew/include/SDL2/SDL.h>  // FIX: change to SDL2/SDL.h

int init_platform(SDL_Window *window, SDL_Renderer *renderer,
                  SDL_Texture *texture, int width, int height, int scale);
void close_platform(SDL_Window *window, SDL_Renderer *renderer,
                    SDL_Texture *texture);

#endif
