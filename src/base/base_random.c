#include <stdlib.h>

#include "base.h"

// Range is inclusive
i32 random_i32(i32 min, i32 max)
{
  // assert(max >= min);

  i32 mult = min < 0 ? 2 : 1;
  return (rand() % ((max - min) * mult + 1)) + min;
}
