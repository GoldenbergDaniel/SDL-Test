#pragma once

#include "../base/base.h"

typedef u8 P_ColliderType;

#define P_ColliderType_Rect 0
#define P_ColliderType_Circle 1

typedef struct P_CollisionParams P_CollisionParams;
struct P_CollisionParams
{
  P_ColliderType type;
  Vec2F pos;
  Vec2F dim;
  Vec2F vel;
  f32 radius;
};

bool p_point_y_range_intersect(P_CollisionParams a, Vec2F range, f32 y);
bool p_rect_x_range_intersect(P_CollisionParams a, Vec2F range, f32 x);
bool p_rect_y_range_intersect(P_CollisionParams a, Vec2F range, f32 y);
bool p_rect_rect_intersect(P_CollisionParams a, P_CollisionParams b);
bool p_rect_circle_intersect(P_CollisionParams a, P_CollisionParams b);
