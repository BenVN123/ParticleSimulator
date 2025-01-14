#ifndef __UTILS__
#define __UTILS__

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

long double calculate_mass(Particle *p);
long double calculate_vel(Particle *p);

#endif
