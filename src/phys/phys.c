#include "../base/base_common.h"
#include "../base/base_math.h"

#include "phys.h"

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

  bool result = FALSE;

  // NOTE(dg): why don't we need velocity?
  Vec2F circle_pos = add_2f(circle.pos, circle.vel);
  // printf("%f\n", magnitude_2f(circle.vel));
  // Vec2F circle_pos = circle.pos;
  Vec2F range_x = {rect.pos.x, rect.pos.x + rect.dim.width};
  Vec2F range_y = {rect.pos.y, rect.pos.y + rect.dim.height};
  Vec2F rect_point = {
    clamp(circle_pos.x, range_x.x, range_x.y), 
    clamp(circle_pos.y, range_y.x, range_y.y), 
  };

  // printf("%f\n", distance_2f(rect_point, circle_pos));
  if (distance_2f(rect_point, circle_pos) <= circle.radius)
  {
    result = TRUE;
  }

  return result;
}
