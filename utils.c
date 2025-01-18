#include "utils.h"

#include <math.h>

#include "particles.h"

double calculate_mass(Particle *p) { return M_PI * pow(p->radius, 2); }

double calculate_vel(Particle *p) {
    return sqrt(pow(p->vel->x, 2) + pow(p->vel->y, 2));
}

double calculate_dist(Particle *p1, Particle *p2) {
    return sqrt(pow(p1->pos->x - p2->pos->x, 2) +
                pow(p1->pos->y - p2->pos->y, 2));
}
