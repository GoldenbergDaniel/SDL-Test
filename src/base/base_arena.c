#include <stdio.h>

#include "base_common.h"
#include "base_os.h"
#include "base_arena.h"

#ifndef SCRATCH_SIZE
#define SCRATCH_SIZE MEGABYTES(4)
#endif

// @Arena ===================================================================================

Arena arena_create(u64 size)
{
  Arena arena;
  arena.memory = os_alloc(size);
  arena.size = size;
  arena.used = 0;

  return arena;
}

void arena_destroy(Arena *arena)
{
  os_free(arena->memory, arena->size);
  arena->memory = NULL;
  arena->size = 0;
  arena->used = 0;
}

void *arena_alloc(Arena *arena, u64 size)
{
  ASSERT(arena->size >= arena->used + size);

  byte *allocated = arena->memory + arena->used;
  arena->used += size;
  
  return allocated;
}

void arena_free(Arena *arena, u64 size)
{
  ASSERT(arena->used - size >= 0);

  arena->used -= size;
}

void arena_clear(Arena *arena)
{
  arena->used = 0;
}

Arena arena_get_scratch(Arena *conflict)
{
  static THREAD_LOCAL Arena scratch_1;
  static THREAD_LOCAL Arena scratch_2;
  static THREAD_LOCAL bool init = TRUE;

  if (init)
  {
    scratch_1 = arena_create(SCRATCH_SIZE);
    scratch_2 = arena_create(SCRATCH_SIZE);
    init = FALSE;
  }

  Arena scratch = scratch_1;
  if (conflict == &scratch_1) scratch = scratch_2;
  else if (conflict == &scratch_2) scratch = scratch_1;

  return scratch;
}

// @ChainArena ==============================================================================

ChainArena chain_arena_create(u64 size)
{
  ChainArena arena;
  arena.regions->prev = NULL;
  arena.regions->memory = os_alloc(size);
  arena.regions->size = size;
  arena.regions->used = 0;

  return arena;
}

void chain_arena_destroy(ChainArena *arena)
{
  for (Arena *curr = arena->regions; curr != NULL;)
  {
    Arena *next = curr->prev;
    os_free(curr->memory, curr->size);
    os_free(curr, sizeof (Arena));
    curr = next;
  }
}

void *chain_arena_alloc(ChainArena *arena, u64 size)
{
  if (arena->regions->size < arena->regions->used + size)
  {
    Arena *new_region = os_alloc(arena->region_size);
    new_region->prev = arena->regions;
    arena->regions = new_region;
    printf("Created a new region!");
  }

  Arena *curr = arena->regions;
  byte *allocated = curr->memory + curr->used;
  curr->used += size;

  return allocated;
}

void chain_arena_clear(ChainArena *arena)
{
  for (Arena *curr = arena->regions; curr != NULL;)
  {
    Arena *next = curr->prev;
    next->used = 0;
    curr = next;
  }
}
