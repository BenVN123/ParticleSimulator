#ifndef __UTILS__
#define __UTILS__

#include </opt/homebrew/include/SDL2/SDL.h>  // FIX: change to SDL2/SDL.h
#include <stdlib.h>

typedef struct Vector {
    long double x;
    long double y;
} Vector;

typedef struct Particle {
    int radius;
    Vector *pos;
    Vector *vel;
    Vector *accel;
} Particle;

typedef struct Segment {
    Vector *pos;
    Particle **particles;
    int p_count;
    int p_limit;
} Segment;

typedef struct Simulator {
    int width;
    int height;
    int scale;
    int fps;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    uint32_t *buffer;
    size_t p_count;
    size_t p_limit;
    Particle **particles;
    long double dt;
    size_t s_count;
    Segment **segments;
} Simulator;

long double calculate_mass(Particle *p);
long double calculate_vel(Particle *p);

#endif
