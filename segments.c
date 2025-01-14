#include "segments.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "particles.h"

int _get_segment_row_count(int width) {
    return ceil((float)width / (SEGMENT_SIDE_LEN));
}

int _get_segment_col_count(int height) {
    return ceil((float)height / (SEGMENT_SIDE_LEN));
}

Segment **generate_segments(int width, int height, int *s_count) {
    int row_s_count = _get_segment_row_count(width);
    *s_count = row_s_count * ceil((float)height / (SEGMENT_SIDE_LEN));
    Segment **segments = malloc(sizeof(Segment *) * (*s_count));

    int row;
    for (int i = 0; i < *s_count; ++i) {
        row = i / row_s_count;
        segments[i] = malloc(sizeof(Segment));
        segments[i]->pos = malloc(sizeof(Vector));
        segments[i]->pos->x = (i % row_s_count) * SEGMENT_SIDE_LEN;
        segments[i]->pos->y = ((int)(i / row_s_count)) * SEGMENT_SIDE_LEN;
        segments[i]->p_count = 0;
        segments[i]->p_limit =
            (row * 20) + 10;  // lower rows have more particles
        segments[i]->particles =
            calloc(segments[i]->p_limit, sizeof(Particle *));
    }

    return segments;
}

Segment *get_segment(Segment **segments, int width, int x, int y) {
    return segments[((y / SEGMENT_SIDE_LEN) * _get_segment_row_count(width)) +
                    (x / SEGMENT_SIDE_LEN)];
}

void update_particle_segment(Segment **segments, int width, int old_x,
                             int old_y, int new_x, int new_y) {
    Segment *prev_seg = get_segment(segments, width, old_x, old_y);
    int p_count = 0;
    Particle *curr_p;
    for (int i = 0; i < prev_seg->p_limit; ++i) {
        curr_p = prev_seg->particles[i];
        if (curr_p->radius != 0) {
            ++p_count;

            if (new_x == (int)curr_p->pos->x && new_y == (int)curr_p->pos->y) {
                curr_p = 0;
                --prev_seg->p_count;
                break;
            } else if (p_count == prev_seg->p_count) {
                printf("smt wrong with segment particles\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    Segment *new_seg = get_segment(segments, width, new_x, new_y);
    Particle *curr_new_p;
    if (new_seg->p_count < new_seg->p_limit) {
        for (int i = 0; i < new_seg->p_limit; ++i) {
            curr_new_p = new_seg->particles[i];
            if (curr_new_p->radius == 0) {
                curr_new_p = curr_p;
            }
        }
    } else {
        new_seg->p_limit *= 2;
        new_seg->particles =
            realloc(new_seg->particles, new_seg->p_limit * sizeof(Particle *));
        new_seg->particles[new_seg->p_count] = curr_p;
        for (int i = new_seg->p_count + 1; i < new_seg->p_limit; ++i) {
            new_seg->particles[i] = 0;
        }
    }
}

void check_segment_collisions(Segment *s1, Segment *s2) {
    int i, j = 0;
    while (i < s1->p_count && i < s1->p_limit) {
        while (j < s2->p_count && j < s2->p_limit) {
            if (check_collision(s1->particles[i], s2->particles[j])) {
                handle_particle_collision(s1->particles[i], s2->particles[j]);
            }
        }
    }
}

void single_segment_check(Segment **segments, int s_idx, int row_count,
                          int col_count) {
    int r_idx = (s_idx + 1) % row_count;
    int b_idx = s_idx + row_count;
    int bl_idx = (s_idx - 1) + row_count;
    int br_idx = s_idx + row_count + 1;

    bool is_left = s_idx % row_count == 0;
    bool is_right = (s_idx + 1) % row_count == 0;
    bool is_bottom = (int)(s_idx / row_count) >= col_count;

    Segment *s = segments[s_idx];

    // check local collisions
    int i = 0;
    int j;
    while (i < s->p_count && i < s->p_limit) {
        j = i + 1;
        while (j < s->p_count && j < s->p_limit) {
            handle_particle_collision(s->particles[i], s->particles[j]);
        }
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

void all_segment_checks(Segment **segments, int s_count, int width,
                        int height) {
    int row_count = _get_segment_row_count(width);
    int col_count = _get_segment_col_count(height);

    for (int i = 0; i < s_count; ++i) {
        single_segment_check(segments, i, row_count, col_count);
    }
}

// TODO: test segment collisions
