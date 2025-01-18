#include "segments.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "particles.h"
#include "utils.h"

int _get_segment_row_count(int width) {
    return ceil((float)width / (SEGMENT_SIDE_LEN));
}

int _get_segment_col_count(int height) {
    return ceil((float)height / (SEGMENT_SIDE_LEN));
}

Segment **generate_segments(Simulator *sim) {
    int row_s_count = _get_segment_row_count(sim->width);
    sim->s_count = row_s_count * ceil((float)sim->height / (SEGMENT_SIDE_LEN));
    Segment **segments = malloc(sizeof(Segment *) * (sim->s_count));

    int row;
    for (int i = 0; i < sim->s_count; ++i) {
        row = i / row_s_count;
        segments[i] = malloc(sizeof(Segment));
        segments[i]->pos = malloc(sizeof(Vector));
        segments[i]->pos->x = (i % row_s_count) * SEGMENT_SIDE_LEN;
        segments[i]->pos->y = ((int)(i / row_s_count)) * SEGMENT_SIDE_LEN;
        segments[i]->p_count = 0;
        segments[i]->p_limit =
            (row * 20) + 10;  // lower rows have more particles
        segments[i]->particles =
            malloc(sizeof(Particle *) * segments[i]->p_limit);
        for (int j = 0; j < segments[i]->p_limit; ++j) {
            segments[i]->particles[j] = NULL;
        }
    }

    return segments;
}

Segment *get_segment(Simulator *sim, int x, int y) {
    return sim->segments[((y / SEGMENT_SIDE_LEN) *
                          _get_segment_row_count(sim->width)) +
                         (x / SEGMENT_SIDE_LEN)];
}

void add_segment_particle(Segment *segment, Particle *particle) {
    Particle *curr_p;
    if (segment->p_count < segment->p_limit) {
        for (int i = 0; i < segment->p_limit; ++i) {
            curr_p = segment->particles[i];
            if (curr_p == NULL) {
                segment->particles[i] = particle;
                break;
            }
        }
    } else {
        segment->p_limit *= 2;
        segment->particles =
            realloc(segment->particles, segment->p_limit * sizeof(Particle *));
        segment->particles[segment->p_count] = curr_p;
        for (int i = segment->p_count + 1; i < segment->p_limit; ++i) {
            segment->particles[i] = NULL;
        }
    }

    ++segment->p_count;
}

// HACK: just use particle struct as argument and store old position, implement
// with verlet
// FIX: not detecting all collisions, might be because segments are not being
// updated correctly
void update_particle_segment(Simulator *sim, Particle *particle, int old_x,
                             int old_y) {
    Segment *prev_seg = get_segment(sim, old_x, old_y);
    for (int i = 0; i < prev_seg->p_limit; ++i) {
        if (prev_seg->particles[i] == particle) {
            prev_seg->particles[i] = NULL;
            --prev_seg->p_count;
            break;
        }
    }

    Segment *new_seg = get_segment(sim, particle->pos->x, particle->pos->y);
    add_segment_particle(new_seg, particle);
}

void check_segment_collisions(Segment *s1, Segment *s2) {
    int i = 0;
    int j = 0;
    while (i < s1->p_count) {
        if (s1->particles[i] != NULL) {
            while (j < s2->p_count) {
                if (s2->particles[j] != NULL) {
                    handle_particle_collision(s1->particles[i],
                                              s2->particles[j]);
                }
                ++j;
            }
        }
        ++i;
    }
}

void single_segment_check(Segment **segments, int s_idx, int row_count,
                          int col_count) {
    int r_idx = (s_idx + 1) % col_count;
    int b_idx = s_idx + col_count;
    int bl_idx = (s_idx - 1) + col_count;
    int br_idx = s_idx + col_count + 1;

    bool is_left = s_idx % col_count == 0;
    bool is_right = (s_idx + 1) % col_count == 0;
    bool is_bottom = (int)(s_idx / col_count) >= row_count - 1;

    Segment *s = segments[s_idx];

    // check local collisions
    int i = 0;
    int j;
    while (i < s->p_count) {
        if (s->particles[i] != NULL) {
            j = i + 1;
            while (j < s->p_count) {
                if (s->particles[j] != NULL) {
                    handle_particle_collision(s->particles[i], s->particles[j]);
                }
                ++j;
            }
        }
        ++i;
    }

    if (!is_bottom) {
        check_segment_collisions(s, segments[b_idx]);

        if (!is_left) {
            check_segment_collisions(s, segments[bl_idx]);
        }

        if (!is_right) {
            check_segment_collisions(s, segments[br_idx]);
        }
    }

    if (!is_right) {
        check_segment_collisions(s, segments[r_idx]);
    }
}

void all_segment_checks(Simulator *sim) {
    int row_count = _get_segment_row_count(sim->width);
    int col_count = _get_segment_col_count(sim->height);

    for (int i = 0; i < sim->s_count; ++i) {
        single_segment_check(sim->segments, i, row_count, col_count);
    }
}
