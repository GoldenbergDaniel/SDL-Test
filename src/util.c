#include <math.h>
#include <stdlib.h>

#include "common.h"
#include "util.h"

// Vector ---------------------------------------------------------------------

inline
Vec2I32 v2i32(i32 x, i32 y)
{
  return (Vec2I32) {x, y};
}

inline
Vec2F32 v2f32(f32 x, f32 y)
{
  return (Vec2F32) {x, y};
}

inline
Vec2I32 add_v2i32(Vec2I32 a, Vec2I32 b)
{
  return (Vec2I32) {a.x + b.x, a.y + b.y};
}

inline
Vec2F32 add_v2f32(Vec2F32 a, Vec2F32 b)
{
  return (Vec2F32) {a.x + b.x, a.y + b.y};
}

inline
Vec2I32 sub_v2i32(Vec2I32 a, Vec2I32 b)
{
  return (Vec2I32) {a.x - b.x, a.y - b.y};
}

inline
Vec2F32 sub_v2f32(Vec2F32 a, Vec2F32 b)
{
  return (Vec2F32) {a.x - b.x, a.y - b.y};
}

inline
Vec2I32 scale_v2i32(Vec2I32 a, f32 s)
{
  return (Vec2I32) {a.x * s, a.y * s};
}

inline
Vec2F32 scale_v2f32(Vec2F32 a, f32 s)
{
  return (Vec2F32) {a.x * s, a.y * s};
}

inline
f32 dot_v2f32(Vec2F32 a, Vec2F32 b)
{
  return (a.x * b.x) + (a.y * b.y);
}


inline
f32 magnitude_v2f32(Vec2F32 a)
{
  return sqrtf(powf(a.x, 2.0f) + powf(a.y, 2.0f));
}

inline
f32 distance_v2f32(Vec2F32 a, Vec2F32 b)
{
  return sqrtf(powf(b.x - a.x, 2.0f) + powf(b.y - a.y, 2.0f));
}

Vec2F32 normalize_v2f32(Vec2F32 a)
{
  f32 mag = magnitude_v2f32(a);
  
  return (Vec2F32) {a.x / mag, a.y / mag};
}

inline
Vec2F32 lerp_v2f(Vec2F32 curr, Vec2F32 target, f32 rate)
{
  return scale_v2f32(sub_v2f32(target, curr), rate);
}

// Other ----------------------------------------------------------------------

inline 
f32 lerp_f32(f32 curr, f32 target, f32 rate)
{
  return curr + (target - curr) * rate;
}

inline
Vec2F32 random_position(u32 min_x, u32 max_x, u32 min_y, u32 max_y)
{
  return (Vec2F32) {
    (rand() % max_x) + min_x,
    (rand() % max_y) + min_y
  };
}

inline
Vec2F32 rect_center(Vec2F32 pos, f32 w, f32 h)
{
  return (Vec2F32) {
    pos.x + (w / 2.0f),
    pos.y + (h / 2.0f)
  };
}

inline
bool range_intersect(f32 min1, f32 max1, f32 min2, f32 max2)
{
  return (max1 >= min2) && (max2 >= min1);
}

bool rect_ranges_intersect(Vec2F32 p1, Vec2F32 p2, f32 w1, f32 h1, f32 w2, f32 h2)
{
  return range_intersect(p1.x, p1.x+w1, p2.x, p2.x+w2) &&
         range_intersect(p1.y, p1.y+h1, p2.y, p2.y+h2);
}

bool rect_intersect(void)
{
  return TRUE;
}
