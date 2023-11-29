#pragma once

#include "base/base_common.h"
#include "base/base_math.h"

typedef struct Collider2D Collider2D;
typedef struct Ray2D Ray2D;

typedef enum ColliderType
{
  ColliderType_Polygon,
  ColliderType_Circle,
} ColliderType;

struct Collider2D
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

struct Ray2D
{
  Vec2F points[2];
  Vec2F dir;
};

bool p_polygon_polygon_intersect(Collider2D *a, Collider2D *b);
Vec2F p_polygon_polygon_resolve(Collider2D *a, Collider2D *b);
bool p_polygon_x_range_intersect(Collider2D *a, Vec2F v1, Vec2F v2);
Vec2F p_polygon_x_range_resolve(Collider2D *a, Vec2F v1, Vec2F v2);
bool p_polygon_y_range_intersect(Collider2D *a, Vec2F v1, Vec2F v2);
Vec2F p_polygon_y_range_resolve(Collider2D *a, Vec2F v1, Vec2F v2);
bool p_polygon_circle_intersect(Collider2D *a, Collider2D *b);
Vec2F p_polygon_circle_resolve(Collider2D *a, Collider2D *b);
bool p_circle_circle_intersect(Collider2D *a, Collider2D *b);
Vec2F p_circle_circle_resolve(Collider2D *a, Collider2D *b);
