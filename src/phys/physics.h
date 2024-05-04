#pragma once

#include "../base/base_common.h"
#include "../base/base_math.h"

typedef u8 P_ColliderType;

#define P_ColliderType_Rect 0
#define P_ColliderType_Circle 1

typedef struct P_Collider P_Collider;
struct P_Collider
{
  P_ColliderType type;
  Vec2F pos;
  Vec2F dim;
  f32 radius;
};

typedef struct P_CollisionParams P_CollisionParams;
struct P_CollisionParams
{
  P_Collider collider;
  Vec2F vel;
};

bool p_rect_x_range_intersect(P_CollisionParams a, Vec2F range, f32 x);
bool p_rect_y_range_intersect(P_CollisionParams a, Vec2F range, f32 y);
bool p_rect_rect_intersect(P_CollisionParams a, P_CollisionParams b);
bool p_rect_circle_intersect(P_CollisionParams a, P_CollisionParams b);
