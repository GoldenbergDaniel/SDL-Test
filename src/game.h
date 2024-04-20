#pragma once

#include "base/base_inc.h"

#include "event.h"
#include "entity.h"

typedef struct Game Game;
struct Game
{
  Arena perm_arena;
  Arena frame_arena;
  Arena batch_arena;
  Arena entity_arena;

  EventQueue event_queue;
  EntityList entities;
  Entity *entity_draw_list;

  Mat3x3F camera;

  f64 t;
  f64 dt;
  
  bool should_quit;
};

void init_game(Game *game);
void update_game(Game *game);
void handle_game_events(Game *game);
void draw_game(Game *game, Game *prev);
bool game_should_quit(Game *game);
void copy_game_state(Game *game, Game *prev);
