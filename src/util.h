#pragma once

#include "common.h"

// Vector ---------------------------------------------------------------------

typedef struct Vec2I32 Vec2I32;
struct Vec2I32
{
  i32 x, y;
};

typedef struct Vec2F32 Vec2F32;
struct Vec2F32
{
  f32 x, y;
};

typedef struct Vec4I8 Vec4I8;
struct Vec4I8
{
  union { i8 x, r; };
  union { i8 y, g; };
  union { i8 z, b; };
  union { i8 w, a; };
};

typedef struct Vec4F32 Vec4F32;
struct Vec4F32
{
  union { f32 x, r; };
  union { f32 y, g; };
  union { f32 z, b; };
  union { f32 w, a; };
};

#define COLOR_BLACK ((Vec4I8) {19, 19, 19, 255})
#define COLOR_WHITE ((Vec4I8) {236, 236, 236, 255})
#define COLOR_RED ((Vec4I8) {249, 58, 38, 255})
#define COLOR_GREEN ((Vec4I8) {100, 160, 60, 255})
#define COLOR_BLUE ((Vec4I8) {55, 107, 186, 255})
#define COLOR_PURPLE ((Vec4I8) {44, 45, 84, 255})

#define VEC2I_ZERO ((Vec2I32) {0, 0})
#define VEC2F_ZERO ((Vec2F32) {0.0f, 0.0f})

Vec2I32 v2i(i32 x, i32 y);
Vec2F32 v2f(f32 x, f32 y);
Vec2I32 add_v2i(Vec2I32 a, Vec2I32 b);
Vec2F32 add_v2f(Vec2F32 a, Vec2F32 b);
Vec2I32 sub_v2i(Vec2I32 a, Vec2I32 b);
Vec2F32 sub_v2f(Vec2F32 a, Vec2F32 b);
Vec2I32 scale_v2i(Vec2I32 a, f32 s);
Vec2F32 scale_v2f(Vec2F32 a, f32 s);
i32 dot_v2i(Vec2I32 a, Vec2I32 b);
f32 dot_v2f(Vec2F32 a, Vec2F32 b);

f32 magnitude_v2i(Vec2I32 a);
f32 magnitude_v2f(Vec2F32 a);
f32 distance_v2i(Vec2I32 a, Vec2I32 b);
f32 distance_v2f(Vec2F32 a, Vec2F32 b);

Vec2F32 normalize_v2i(Vec2I32 a);
Vec2F32 normalize_v2f(Vec2F32 a);

Vec2F32 lerp_v2f(Vec2F32 curr, Vec2F32 target, f32 rate);

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

Vec2F32 random_position(u32 min_x, u32 max_x, u32 min_y, u32 max_y);
Vec2F32 rect_center(Vec2F32 pos, f32 w, f32 h);

bool range_intersect(f32 min1, f32 max1, f32 min2, f32 max2);
bool rect_ranges_intersect(Vec2F32 p1, Vec2F32 p2, f32 w1, f32 h1, f32 w2, f32 h2);
bool rect_intersect(void);
