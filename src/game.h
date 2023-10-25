#pragma once

#include "base/base_common.h"
#include "base/base_arena.h"
#include "gfx/draw.h"
#include "event.h"
#include "entity.h"

#define COLOR_BLACK ((Vec4F) {0.05f, 0.05f, 0.05f, 1.0f})
#define COLOR_WHITE ((Vec4F) {0.9f, 0.9f, 0.9f, 1.0f})
#define COLOR_RED ((Vec4F) {0.9f, 0.2f, 0.1f, 1.0f})
#define COLOR_YELLOW ((Vec4F) {0.9f, 0.8f, 0.0f, 1.0f})
#define COLOR_BLUE ((Vec4F) {0.1f, 0.4f, 0.8f, 1.0f})

typedef union SDL_Event SDL_Event;

typedef struct Game Game;
struct Game
{
  Arena arena;
  EventQueue event_queue;
  Mat3x3F camera;
  bool running;
  bool first_frame;
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
void handle_events(Game *game);
bool should_quit(void);
Entity *get_player(Game *game);
Entity *spawn_laser(Game *game, Vec2F pos, f32 rot);
void handle_input(Game *game, SDL_Event *event);
