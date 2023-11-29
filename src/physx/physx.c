#include "base/base_common.h"
#include "base/base_math.h"

#include "physx.h"

// https://www.youtube.com/watch?v=7Ik2vowGcU0
bool p_polygon_polygon_intersect(Collider2D *a, Collider2D *b)
{
  return FALSE;
}

Vec2F p_polygon_polygon_resolve(Collider2D *a, Collider2D *b)
{
  return V2F_ZERO;
}

bool p_polygon_y_range_intersect(Collider2D *a, Vec2F v1, Vec2F v2)
{
  bool result = FALSE;

  for (u8 i = 0; i < a->vertex_count; i++)
  {
    if (a->vertices[a->edges[i][0]].x >= v1.x && 
        a->vertices[a->edges[i][1]].x <= v2.x)
    {
      if (a->vertices[a->edges[i][0]].y <= v1.y && 
          a->vertices[a->edges[i][1]].y <= v2.y)
      {
        result = TRUE;
        break;
      }
    }
  }

  return result;
}

Vec2F p_polygon_range_resolve(Collider2D *a, Vec2F v1, Vec2F v2)
{
  return V2F_ZERO;
}

bool p_polygon_circle_intersect(Collider2D *a, Collider2D *b)
{
  return FALSE;
}

Vec2F p_polygon_circle_resolve(Collider2D *a, Collider2D *b)
{
  return V2F_ZERO;
}

bool p_circle_circle_intersect(Collider2D *a, Collider2D *b)
{
  return FALSE;
}

Vec2F p_circle_circle_resolve(Collider2D *a, Collider2D *b)
{
  return V2F_ZERO;
}
