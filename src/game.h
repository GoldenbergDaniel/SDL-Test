#pragma once

#include "base/base_common.h"
#include "base/base_arena.h"
#include "gfx/draw.h"
#include "entity.h"

typedef union SDL_Event SDL_Event;

typedef struct Game Game;
struct Game
{
  Arena arena;
  Mat3x3F camera;
  bool running : 1;
  bool first_frame : 1;
  f64 t;
  f64 dt;
  Entity *entity_head;
  Entity *entity_tail;
  Entity *entity_first_free;
  u16 entity_count;
};

void init(Game *game);
void update(Game *game);
void draw(Game *game);
bool should_quit(void);
Entity *get_player(Game *game);
Entity *spawn_laser(Game *game, Vec2F pos, f32 rot);
void handle_event(Game *game, SDL_Event *event);
