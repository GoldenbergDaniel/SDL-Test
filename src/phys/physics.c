#include "../base/base_common.h"

#include "physics.h"

typedef P_Collider Collider;
typedef P_CollisionParams CollisionParams;

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
    // bool top = (a.collider.pos.y + a.vel.y + a.collider.dim.height * 0.5f) == y;
    bool top = 0;
    bool bot = (a.collider.pos.y + a.vel.y - a.collider.dim.height) <= y;
    result = top || bot;
  }

  return result;
}

bool p_rect_rect_intersect(CollisionParams a, CollisionParams b)
{
  bool result = FALSE;

  return result;
}
