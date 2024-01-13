#include <stdlib.h>

#include "base_common.h"
#include "base_random.h"

u32 random_u32(u32 min, u32 max)
{
  return rand() % max + min;
}

inline
u64 random_u64(u64 min, u64 max)
{
  return rand() % max + min;
}
