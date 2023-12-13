#pragma once

#include "base/base_common.h"
#include "base/base_arena.h"
#include "gfx/draw.h"
#include "input.h"
#include "event.h"
#include "entity.h"

#define TARGET_FPS 60

#define VSYNC_AUTO -1
#define VSYNC_OFF 0
#define VSYNC_ON 1

#define W_WIDTH 1024
#define W_HEIGHT 640

#define COLOR_BLACK ((Vec4F) {0.01f, 0.01f, 0.01f, 1.0f})
#define COLOR_WHITE ((Vec4F) {0.9f, 0.9f, 0.9f, 1.0f})
#define COLOR_GRAY ((Vec4F) {0.5f, 0.5f, 0.5f, 1.0f})
#define COLOR_RED ((Vec4F) {0.9f, 0.2f, 0.1f, 1.0f})
#define COLOR_GREEN ((Vec4F) {0.3f, 0.9f, 0.2f, 1.0f})
#define COLOR_BLUE ((Vec4F) {0.1f, 0.4f, 0.8f, 1.0f})
#define COLOR_YELLOW ((Vec4F) {0.9f, 0.8f, 0.0f, 1.0f})

#define GRAVITY 20.0f

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
  D_Assets assets;
  D_Renderer renderer;
  Entity *nil_entity;
};

void init_game(Game *game);
void update_game(Game *game);
void handle_game_events(Game *game);
void draw_game(Game *game);
void clear_game_frame_arena(Game *game);
bool game_should_quit(Game *game);
