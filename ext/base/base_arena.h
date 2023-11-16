#pragma once

#include "base_common.h"

#define KiB(bytes) (bytes << 10)
#define MiB(bytes) (bytes << 20)
#define GiB(bytes) (bytes << 30)

typedef struct Arena Arena;
struct Arena
{
  Arena *prev;
  byte *memory;
  u64 size;
  u64 used;
};

Arena create_arena(u64 size);
void destroy_arena(Arena *arena);
void *arena_alloc(Arena *arena, u64 size);
void arena_free(Arena *arena, u64 size);
void clear_arena(Arena *arena);
Arena get_scratch_arena(Arena *conflict);
