#include "../base/base_common.h"
#include "../base/base_math.h"

#include "phys.h"

typedef P_Collider Collider;
typedef P_CollisionParams CollisionParams;

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
  assert(a.collider.type == P_ColliderType_Rect);

  return FALSE;
}

bool p_rect_y_range_intersect(CollisionParams a, Vec2F range, f32 y)
{
  assert(a.collider.type == P_ColliderType_Rect);

  bool result = FALSE;

  if (a.collider.pos.x >= range.x && a.collider.pos.x + a.collider.dim.width * 0.5f <= range.y)
  {
    // bool top = (a.collider.pos.y + a.vel.y + a.collider.dim.height * 0.5f) >= y;
    bool top = 0;
    bool bot = (a.collider.pos.y + a.vel.y) <= y;
    result = top || bot;
  }

  return result;
}

bool p_rect_circle_intersect(CollisionParams rect, CollisionParams circle)
{
  assert(rect.collider.type == P_ColliderType_Rect && 
         circle.collider.type == P_ColliderType_Circle);

  bool result = FALSE;

  // (@dg): why don't we need velocity?
  Vec2F circle_pos = circle.collider.pos;
  Vec2F range_x = {rect.collider.pos.x, rect.collider.pos.x + rect.collider.dim.width};
  Vec2F range_y = {rect.collider.pos.y, rect.collider.pos.y + rect.collider.dim.height};
  Vec2F rect_point = {
    clamp(circle_pos.x, range_x.x, range_x.y), 
    clamp(circle_pos.y, range_y.x, range_y.y), 
  };

  if (distance_2f(rect_point, circle_pos) <= circle.collider.radius)
  {
    result = TRUE;
  }

  return result;
}
