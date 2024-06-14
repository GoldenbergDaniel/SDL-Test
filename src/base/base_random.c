#include <stdlib.h>

#include "base.h"

// Range is inclusive
u64 random_u64(u64 min, u64 max)
{
  return (rand() % ((max - min) + 1)) + min;
}

i32 random_i32(i32 min, i32 max)
{
  i32 mult = min < 0 ? 2 : 1;

  return (rand() % ((max - min) * mult + 1)) + min;
}
