#pragma once

#include <SDL2/SDL.h>

#include "base/base_common.h"
#include "base/base_arena.h"
#include "draw.h"
#include "entity.h"

typedef struct Game Game;
struct Game
{
  Arena arena;
  Mat3x3F camera;
  bool running : 1;
  bool first_frame : 1;
  f64 t;
  f64 dt;
  Entity *player;
  Entity entities[512];
  u16 entity_count;
};

void game_init(Game *game);
void game_handle_event(Game *game, SDL_Event *event);
void game_update(Game *game);
void game_draw(Game *game);
bool game_should_quit(void);
