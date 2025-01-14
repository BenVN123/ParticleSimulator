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

Segment **generate_segments(int width, int height, int *s_count);
Segment *get_segment(Segment **segments, int width, int x, int y);
void update_particle_segment(Segment **segments, int width, int old_x,
                             int old_y, int new_x, int new_y);
void check_segment_collisions(Segment *s1, Segment *s2);
void single_segment_check(Segment **segments, int s_idx, int row_count,
                          int col_count);
void all_segment_checks(Segment **segments, int s_count, int width, int height);

#endif
