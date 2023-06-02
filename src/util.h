#pragma once

#define len(arr) sizeof(arr)/sizeof(arr[0])

typedef struct v2
{
    f32 x;
    f32 y;
} v2;

#define V2_ZERO (v2) {0.0f, 0.0f}

i32 round_f32(f32 num);
f32 lerp(f32 curr, f32 target, f32 rate);
f32 magnitude(v2 vec);
f32 distance(v2 vec1, v2 vec2);
v2 add_v2(v2 vec1, v2 vec2);
v2 normalize_v2(v2 vec);
v2 get_random_position(i32 min_x, i32 max_x, i32 min_y, i32 max_y);
v2 get_screen_center(u16 w, u16 h);
v2 get_rect_center(v2 pos, u16 w, u16 h);
bool range_intersect(f32 min1, f32 max1, f32 min2, f32 max2);
bool rr_collision(v2 pos1, v2 pos2, f32 w1, f32 h1, f32 w2, f32 h2);
