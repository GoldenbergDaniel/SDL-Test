#include "base/base_inc.h"

#include "global.h"

Vec2F screen_to_world(Vec2F pos)
{
  return v2f(pos.x, HEIGHT - pos.y);
}
