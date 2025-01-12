#ifndef __SEGMENTS__
#define __SEGMENTS__

#include "particles.h"

#define SEGMENT_SIDE_LEN ((MAX_RADIUS * 2) + 1)

typedef struct Segment {
    Vector *pos;
    Particle **particles;
    int p_count;
    int p_limit;
} Segment;

int get_segment_row_count(int width);
Segment **generate_segment(int width, int height, int *s_count);
void update_particle_segment(Segment **segments, int scr_width, int old_x,
                             int old_y, int new_x, int new_y);

#endif
