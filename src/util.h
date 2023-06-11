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

#define abs(a) (((a) < 0) ? (-a) : (a))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define clamp_top(a, b) (min(a, b))
#define clamp_bottom(a, b) (max(a, b))
#define round(a) ((i32) (a + 0.5f))

#define array_len(arr) (sizeof(arr) / sizeof(arr[0]))

f32 lerp_f32(f32 curr, f32 target, f32 rate);
v2f lerp_v2f(v2f curr, v2f target, f32 rate);

v2i add_v2i(v2i a, v2i b);
v2f add_v2f(v2f a, v2f b);
v2i subtract_v2i(v2i a, v2i b);
v2f subtract_v2f(v2f a, v2f b);
v2i scale_v2i(v2i a, f32 s);
v2f scale_v2f(v2f a, f32 s);
i32 dot_v2i(v2i a, v2i b);
f32 dot_v2f(v2f a, v2f b);

f32 magnitude_v2i(v2i a);
f32 magnitude_v2f(v2f a);
f32 distance_v2i(v2i a, v2i b);
f32 distance_v2f(v2f a, v2f b);

v2f normalize_v2i(v2i a);
v2f normalize_v2f(v2f a);

v2f get_random_position(u32 min_x, u32 max_x, u32 min_y, u32 max_y);
v2f get_screen_center(u16 w, u16 h);
v2f get_rect_center(v2f pos, u16 w, u16 h);

bool range_intersect(f32 min1, f32 max1, f32 min2, f32 max2);
bool rr_collision(v2f pos1, v2f pos2, f32 w1, f32 h1, f32 w2, f32 h2);
