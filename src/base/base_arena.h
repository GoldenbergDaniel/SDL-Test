#pragma once

#include "base_common.h"

#define KiB(bytes) (bytes << 10)
#define MiB(bytes) (bytes << 20)
#define GiB(bytes) (bytes << 30)

typedef struct Arena Arena;
struct Arena
{
  u8 *memory;
  u64 size;
  u64 used;
};

Arena create_arena(u64 size);
void destroy_arena(Arena *arena);
void *arena_push(Arena *arena, u64 size);
void arena_pop(Arena *arena, u64 size);
void arena_clear(Arena *arena);
Arena get_scratch_arena(Arena *conflict);
