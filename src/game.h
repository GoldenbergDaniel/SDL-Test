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
  Mat3x3F projection;
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

typedef struct EntitySpawnParams EntitySpawnParams;
struct EntitySpawnParams
{
  b64 props;
  Vec2F pos;
  Vec4F color;
};

typedef struct EntityKillParams EntityKillParams;
struct EntityKillParams
{
  Entity *entity;
  u64 id;
};

#define spawn_entity(game, type, ...) \
  _spawn_entity(game, type, (EntitySpawnParams) \
  {.pos=v2f(0, 0), .color=D_WHITE, __VA_ARGS__ })

#define kill_entity(game, ...) \
  _kill_entity(game, (EntityKillParams) \
  {.entity=NULL, .id=0, __VA_ARGS__ })

Entity *_spawn_entity(Game *game, EntityType type, EntitySpawnParams params);
void _kill_entity(Game *game, EntityKillParams params);
Vec2F screen_to_world(Vec2F pos);
