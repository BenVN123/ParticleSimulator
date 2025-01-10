#include "utils.h"

#include <math.h>

#include "particles.h"

long double calculate_mass(Particle *p) { return M_PI * pow(p->radius, 2); }

long double calculate_vel(Particle *p) {
    return sqrt(pow(p->vel->x, 2) + pow(p->vel->y, 2));
}
