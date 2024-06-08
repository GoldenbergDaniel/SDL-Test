#include "../base/base.h"
#include "../vecmath/vecmath.h"
#include "physics.h"

typedef P_CollisionParams CollisionParams;

// NOTE(dg): Assume bottem-left origin

bool p_point_y_range_intersect(P_CollisionParams a, Vec2F range, f32 y)
{
  bool result = FALSE;

  if (a.pos.x >= range.x && a.pos.x <= range.y)
  {
    result = a.pos.y + a.vel.y <= y;
  }

  return result;
}

bool p_rect_x_range_intersect(CollisionParams a, Vec2F range, f32 x)
{
  assert(a.type == P_ColliderType_Rect);

  return FALSE;
}

bool p_rect_y_range_intersect(CollisionParams a, Vec2F range, f32 y)
{
  assert(a.type == P_ColliderType_Rect);

  bool result = FALSE;

  if (a.pos.x >= range.x && a.pos.x + a.dim.width * 0.5f <= range.y)
  {
    result = (a.pos.y + a.vel.y) <= y;
  }

  return result;
}

bool p_rect_circle_intersect(CollisionParams rect, CollisionParams circle)
{
  assert(rect.type == P_ColliderType_Rect && circle.type == P_ColliderType_Circle);

  Vec2F circle_pos = add_2f(circle.pos, circle.vel);
  Vec2F x_range = {rect.pos.x, rect.pos.x + rect.dim.width};
  Vec2F y_range = {rect.pos.y, rect.pos.y + rect.dim.height};

  Vec2F rect_point = {
    clamp(circle_pos.x, x_range.x, x_range.y), 
    clamp(circle_pos.y, y_range.x, y_range.y), 
  };

  return distance_2f(rect_point, circle_pos) <= circle.radius;
}

bool p_rect_rect_intersect(P_CollisionParams a, P_CollisionParams b)
{
  assert(a.type == P_ColliderType_Rect && b.type == P_ColliderType_Rect);

  Vec2F pos_a = add_2f(a.pos, a.vel);
  Vec2F pos_b = add_2f(b.pos, b.vel);

  bool x_range = (pos_a.x <= pos_b.x && pos_a.x + a.dim.width >= pos_b.x) ||
                 (pos_b.x <= pos_a.x && pos_b.x + b.dim.width >= pos_a.x);
  
  bool y_range = (pos_a.y <= pos_b.y && pos_a.y + a.dim.height >= pos_b.y) ||
                 (pos_b.y <= pos_a.y && pos_b.y + b.dim.height >= pos_a.y);  

  return x_range && y_range;
}
