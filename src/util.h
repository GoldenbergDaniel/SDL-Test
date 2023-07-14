#pragma once

// Vector ---------------------------------------------------------------------

typedef struct Vec2I Vec2I;
struct Vec2I
{
  i32 x, y;
};

typedef struct Vec2F Vec2F;
struct Vec2F
{
  f32 x, y;
};

#define VEC2I_ZERO ((Vec2I) {0, 0})
#define VEC2F_ZERO ((Vec2F) {0.0f, 0.0f})

Vec2I v2i(i32 x, i32 y);
Vec2F v2f(f32 x, f32 y);
Vec2I add_v2i(Vec2I a, Vec2I b);
Vec2F add_v2f(Vec2F a, Vec2F b);
Vec2I sub_v2i(Vec2I a, Vec2I b);
Vec2F sub_v2f(Vec2F a, Vec2F b);
Vec2I scale_v2i(Vec2I a, f32 s);
Vec2F scale_v2f(Vec2F a, f32 s);
i32 dot_v2i(Vec2I a, Vec2I b);
f32 dot_v2f(Vec2F a, Vec2F b);

f32 magnitude_v2i(Vec2I a);
f32 magnitude_v2f(Vec2F a);
f32 distance_v2i(Vec2I a, Vec2I b);
f32 distance_v2f(Vec2F a, Vec2F b);

Vec2F normalize_v2i(Vec2I a);
Vec2F normalize_v2f(Vec2F a);

Vec2F lerp_v2f(Vec2F curr, Vec2F target, f32 rate);

// Color ----------------------------------------------------------------------

typedef struct ColorI8 ColorI8;
struct ColorI8
{
  i8 r, g, b, a;
};

typedef struct ColorF32 ColorF32;
struct ColorF32
{
  f32 r, g, b, a;
};

#define COLOR_BLACK ((ColorI8) {19, 19, 19, 255})
#define COLOR_WHITE ((ColorI8) {236, 236, 236, 255})
#define COLOR_RED ((ColorI8) {249, 58, 38, 255})
#define COLOR_GREEN ((ColorI8) {100, 160, 60, 255})
#define COLOR_BLUE ((ColorI8) {55, 107, 186, 255})
#define COLOR_PURPLE ((ColorI8) {44, 45, 84, 255})

// Input ----------------------------------------------------------------------

typedef struct Input Input;
struct Input
{
  u8 w;
  u8 a;
  u8 s;
  u8 d;
  u8 space;
  u8 escape;
};

// Other ----------------------------------------------------------------------

#define abs(a) (((a) < 0) ? (-(a)) : (a))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define clamp_top(a, b) (min(a, b))
#define clamp_bottom(a, b) (max(a, b))
#define round(a) ((i32) (a) + 0.5f))

#define arr_len(arr) (sizeof (arr) / sizeof (arr[0]))

f32 lerp_f32(f32 curr, f32 target, f32 rate);

Vec2F random_position(u32 min_x, u32 max_x, u32 min_y, u32 max_y);
Vec2F rect_center(Vec2F pos, f32 w, f32 h);

bool range_intersect(f32 min1, f32 max1, f32 min2, f32 max2);
bool rect_ranges_intersect(Vec2F p1, Vec2F p2, f32 w1, f32 h1, f32 w2, f32 h2);
bool rect_intersect(void);
