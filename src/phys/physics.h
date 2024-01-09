#pragma once

#include "base/base_common.h"
#include "base/base_math.h"

typedef struct P_Collider P_Collider;
typedef enum P_ColliderType
{
  ColliderType_Polygon,
  ColliderType_Circle,
} ColliderType;

struct P_Collider
{
  ColliderType type;
  Vec2F pos;
  f32 rot;

  Vec2F vertices[4];
  u8 edges[4][2];
  Vec2F normals[4];
  u8 vertex_count;

  u16 radius;
};

typedef struct P_Ray P_Ray;
struct P_Ray
{
  Vec2F points[2];
  Vec2F dir;
};

bool p_polygon_polygon_intersect(P_Collider *a, P_Collider *b);
Vec2F p_polygon_polygon_resolve(P_Collider *a, P_Collider *b);
bool p_polygon_x_range_intersect(P_Collider *a, Vec2F v1, Vec2F v2);
Vec2F p_polygon_x_range_resolve(P_Collider *a, Vec2F v1, Vec2F v2);
bool p_polygon_y_range_intersect(P_Collider *a, Vec2F v1, Vec2F v2);
Vec2F p_polygon_y_range_resolve(P_Collider *a, Vec2F v1, Vec2F v2);
bool p_polygon_circle_intersect(P_Collider *a, P_Collider *b);
Vec2F p_polygon_circle_resolve(P_Collider *a, P_Collider *b);
bool p_circle_circle_intersect(P_Collider *a, P_Collider *b);
Vec2F p_circle_circle_resolve(P_Collider *a, P_Collider *b);
