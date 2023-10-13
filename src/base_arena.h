#pragma once

#include "base_common.h"

#define KILOBYTES(bytes) (bytes << 10)
#define MEGABYTES(bytes) (bytes << 20)
#define GIGABYTES(bytes) (bytes << 30)

// @Arena ===================================================================================

typedef struct Arena Arena;
struct Arena
{
  Arena *prev;
  byte *memory;
  u64 size;
  u64 used;
};

Arena arena_create(u64 size);
void arena_destroy(Arena *arena);
void *arena_alloc(Arena *arena, u64 size);
void arena_free(Arena *arena, u64 size);
void arena_clear(Arena *arena);
Arena arena_get_scratch(Arena *conflict);

// @ChainArena ==============================================================================

typedef struct ChainArena ChainArena;
struct ChainArena
{
  Arena *regions;
  u64 region_size;
};

ChainArena chain_arena_create(u64 region_size);
void chain_arena_destroy(ChainArena *arena);
void *chain_arena_alloc(ChainArena *arena, u64 size);
void chain_arena_clear(ChainArena *arena);
