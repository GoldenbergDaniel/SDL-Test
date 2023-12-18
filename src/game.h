#pragma once

#include "base/base_common.h"
#include "base/base_arena.h"
#include "gfx/draw.h"
#include "input.h"
#include "event.h"
#include "entity.h"

#define WIDTH 1024
#define HEIGHT 640

typedef struct Game Game;
typedef struct Global Global;

struct Game
{
  Arena perm_arena;
  Arena frame_arena;
  Arena entity_arena;
  EventQueue event_queue;
  EntityList entities;
  Mat3x3F camera;
  f64 t;
  f64 dt;
  bool should_quit;
};

struct Global
{
  Input input;
  D_Resources resources;
  D_Renderer renderer;
  Entity *nil_entity;
};

void init_game(Game *game);
void update_game(Game *game);
void handle_game_events(Game *game);
void draw_game(Game *game);
bool game_should_quit(Game *game);
