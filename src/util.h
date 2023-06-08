#pragma once

typedef struct v2i
{
    i32 x, y;
} v2i;

typedef struct v2f
{
    f32 x, y;
} v2f;

#define V2I_ZERO ((v2f) {0, 0})
#define V2F_ZERO ((v2f) {0.0f, 0.0f})

#define round(num) ((i32) (num + 0.5f))

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define clamp_top(a, b) (min(a, b))
#define clamp_bottom(a, b) (max(a, b))

#define array_len(array) (sizeof(array) / sizeof(array[0]))

f32 lerp(f32 curr, f32 target, f32 rate);

f32 magnitude_v2i(v2i vec);
f32 magnitude_v2f(v2f vec);
f32 distance_v2i(v2i vec1, v2i vec2);
f32 distance_v2f(v2f vec1, v2f vec2);

v2i add_v2i(v2i vec1, v2i vec2);
v2f add_v2f(v2f vec1, v2f vec2);
i32 dot_v2i(v2i vec1, v2i vec2);
f32 dot_v2f(v2f vec1, v2f vec2);
v2f normalize_v2f(v2f vec);

v2f get_random_position(u32 min_x, u32 max_x, u32 min_y, u32 max_y);
v2f get_screen_center(u16 w, u16 h);
v2f get_rect_center(v2f pos, u16 w, u16 h);

bool range_intersect(f32 min1, f32 max1, f32 min2, f32 max2);
bool rr_collision(v2f pos1, v2f pos2, f32 w1, f32 h1, f32 w2, f32 h2);
