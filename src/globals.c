#include "base/base_common.h"
#include "globals.h"

extern Globals *GLOBAL;

inline
f32 get_width(void)
{
  return GLOBAL->viewport.z;
}

inline
f32 get_height(void)
{
  return GLOBAL->viewport.w;
}
