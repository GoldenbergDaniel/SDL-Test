#include <stdlib.h>

#include "base_common.h"
#include "base_random.h"

u64 random_u64(u64 min, u64 max)
{
  return (rand() % max) + min;
}

i32 random_i32(i32 min, i32 max)
{
  return (rand() % (max * 2 + 1)) + min;
}
