#include <math.h>
#include <stdlib.h>

#include "globals.h"
#include "util.h"

// Vector ---------------------------------------------------------------------

inline
Vec2I v2i(i32 x, i32 y)
{
  return (Vec2I) {x, y};
}

inline
Vec2F v2f(f32 x, f32 y)
{
  return (Vec2F) {x, y};
}

inline
Vec2I add_v2i(Vec2I a, Vec2I b)
{
  return (Vec2I) {a.x + b.x, a.y + b.y};
}

inline
Vec2F add_v2f(Vec2F a, Vec2F b)
{
  return (Vec2F) {a.x + b.x, a.y + b.y};
}

inline
Vec2I sub_v2i(Vec2I a, Vec2I b)
{
  return (Vec2I) {a.x - b.x, a.y - b.y};
}

inline
Vec2F sub_v2f(Vec2F a, Vec2F b)
{
  return (Vec2F) {a.x - b.x, a.y - b.y};
}

inline
Vec2I scale_v2i(Vec2I a, f32 s)
{
  return (Vec2I) {a.x * s, a.y * s};
}

inline
Vec2F scale_v2f(Vec2F a, f32 s)
{
  return (Vec2F) {a.x * s, a.y * s};
}

inline
i32 dot_v2i(Vec2I a, Vec2I b)
{
  return (a.x * b.x) + (a.y * b.y);
}

inline
f32 dot_v2f(Vec2F a, Vec2F b)
{
  return (a.x * b.x) + (a.y * b.y);
}

inline
f32 magnitude_v2i(Vec2I a)
{
  return sqrtf(powf(a.x, 2.0f) + powf(a.y, 2.0f));
}

inline
f32 magnitude_v2f(Vec2F a)
{
  return sqrtf(powf(a.x, 2.0f) + powf(a.y, 2.0f));
}

inline
f32 distance_v2i(Vec2I a, Vec2I b)
{
  return sqrtf(powf(b.x - a.x, 2.0f) + powf(b.y - a.y, 2.0f));
}

inline
f32 distance_v2f(Vec2F a, Vec2F b)
{
  return sqrtf(powf(b.x - a.x, 2.0f) + powf(b.y - a.y, 2.0f));
}

Vec2F normalize_v2i(Vec2I a)
{
  f32 mag = magnitude_v2i(a);
  
  return (Vec2F) {a.x / mag, a.y / mag};
}

Vec2F normalize_v2f(Vec2F a)
{
  f32 mag = magnitude_v2f(a);
  
  return (Vec2F) {a.x / mag, a.y / mag};
}

inline
Vec2F lerp_v2f(Vec2F curr, Vec2F target, f32 rate)
{
  return scale_v2f(sub_v2f(target, curr), rate);
}

// Other ----------------------------------------------------------------------

inline 
f32 lerp_f32(f32 curr, f32 target, f32 rate)
{
  return (target - curr) * rate;
}

Vec2F random_position(u32 min_x, u32 max_x, u32 min_y, u32 max_y)
{
  return (Vec2F) {
    (rand() % max_x) + min_x,
    (rand() % max_y) + min_y
  };
}

inline
Vec2F rect_center(Vec2F pos, f32 w, f32 h)
{
  return (Vec2F) {
    pos.x + (w / 2.0f),
    pos.y + (h / 2.0f)
  };
}

inline
bool range_intersect(f32 min1, f32 max1, f32 min2, f32 max2)
{
  return (max1 >= min2) && (max2 >= min1);
}

bool rect_ranges_intersect(Vec2F p1, Vec2F p2, f32 w1, f32 h1, f32 w2, f32 h2)
{
  return range_intersect(p1.x, p1.x+w1, p2.x, p2.x+w2) &&
         range_intersect(p1.y, p1.y+h1, p2.y, p2.y+h2);
}

bool rect_intersect(void)
{
  return TRUE;
}
