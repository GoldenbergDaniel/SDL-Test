#pragma once

#include <SDL2/SDL.h>

#include "base/base_common.h"
#include "base/base_arena.h"
#include "draw.h"
#include "entity.h"

typedef struct Game Game;
struct Game
{
  SDL_Window *window;
  Arena arena;
  Mat3x3F camera;
  bool running;
  bool first_frame;
  f64 t;
  f64 dt;
  u8 entity_count;
  u8 enemy_count;
  Entity entities[512];
  Entity *player;
};

typedef struct State State;
struct State
{
  Input *input;
  D_Renderer *renderer;
};

void game_init(Game *game);
void game_handle_event(Game *game, SDL_Event *event);
void game_update(Game *game);
void game_draw(Game *game);
bool game_should_quit(void);
