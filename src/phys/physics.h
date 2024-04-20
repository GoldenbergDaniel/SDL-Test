#pragma once

#include "../base/base_common.h"
#include "../base/base_math.h"

typedef enum P_ColliderType
{
  P_ColliderType_Polygon,
  P_ColliderType_Circle,
} P_ColliderType;

typedef struct P_Collider P_Collider;
struct P_Collider
{
  P_ColliderType type;
  Vec2F pos;
  f32 rot;

  Vec2F vertices[4];
  u8 edges[4][2];
  Vec2F normals[4];
  u8 vertex_count;

  f32 radius;
};

bool p_rect_x_range_intersect(P_Collider *a, Vec2F v1, Vec2F v2);
Vec2F p_rect_x_range_resolve(P_Collider *a, Vec2F v1, Vec2F v2);
bool p_rect_y_range_intersect(P_Collider *a, Vec2F v1, Vec2F v2);
Vec2F p_rect_y_range_resolve(P_Collider *a, Vec2F v1, Vec2F v2);
bool p_rect_rect_intersect(P_Collider *a, P_Collider *b);
Vec2F p_rect_rect_resolve(P_Collider *a, P_Collider *b);
bool p_rect_circle_intersect(P_Collider *a, P_Collider *b);
Vec2F p_rect_circle_resolve(P_Collider *a, P_Collider *b);
