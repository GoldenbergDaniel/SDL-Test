#include "../base/base_common.h"
#include "../base/base_math.h"

#include "physics.h"

typedef P_Collider Collider;

bool p_rect_y_range_intersect(Collider *a, Vec2F v1, Vec2F v2)
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

bool p_rect_rect_intersect(P_Collider *a, P_Collider *b)
{
  bool result = FALSE;


  
  return result;
}
