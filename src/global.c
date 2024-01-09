#include "base/base_common.h"

#include "global.h"

Vec2F screen_to_world(Vec2F pos)
{
  Vec3F result = v3f(pos.x, pos.y, 1);
  result.y = HEIGHT - pos.y; 

  return v2f(result.x, result.y);
}
