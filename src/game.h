#pragma once

#include "base/base_common.h"
#include "base/base_arena.h"
#include "gfx/draw.h"
#include "input.h"
#include "event.h"
#include "entity.h"

typedef struct Game Game;
struct Game
{
  Arena perm_arena;
  Arena frame_arena;
  Arena entity_arena;
  EventQueue event_queue;
  EntityList entities;
  Mat3x3F camera;
  Mat3x3F projection;
  f64 t;
  f64 dt;
  bool should_quit;
};

void init_game(Game *game);
void update_game(Game *game);
void handle_game_events(Game *game);
void draw_game(Game *game);
bool game_should_quit(Game *game);
