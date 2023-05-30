#pragma once

#define UTIL_H

#define len(arr) sizeof(arr)/sizeof(arr[0])

typedef struct v2
{
    f32 x;
    f32 y;
} v2;

typedef struct vi2
{
    i32 x;
    i32 y;
} vi2;

f32 lerp(f32 curr, f32 target, f32 rate);
f32 magnitude(v2 vec);
v2 add_v2(v2 vec1, v2 vec2);
v2 normalize_v2(v2 vec);
v2 get_rect_center(u16 w, u16 h);
v2 get_random_position(i32 min_x, i32 max_x, i32 min_y, i32 max_y);
bool range_intersect(f32 min1, f32 max1, f32 min2, f32 max2);
bool rr_collision(v2 pos1, v2 pos2, f32 w1, f32 h1, f32 w2, f32 h2);
