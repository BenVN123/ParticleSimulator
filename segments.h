#ifndef __SEGMENTS__
#define __SEGMENTS__

#include "utils.h"

#define SEGMENT_SIDE_LEN ((MAX_RADIUS * 2) + 1)

Segment **generate_segments(Simulator *sim);
Segment *get_segment(Simulator *sim, int x, int y);
void add_segment_particle(Segment *segment, Particle *particle);
void update_particle_segment(Simulator *sim, Particle *particle, int old_x,
                             int old_y);
void check_segment_collisions(Segment *s1, Segment *s2);
void single_segment_check(Segment **segments, int s_idx, int row_count,
                          int col_count);
void all_segment_checks(Simulator *sim);

#endif
