#pragma once

#include "base/base_inc.h"

#include "input.h"
#include "entity.h"

#ifdef _WIN32
#define TIME_STEP (1.0f / 120)
#define ANIM_TICK 1
#else
#define TIME_STEP (1.0f / 60)
#define ANIM_TICK 2
#endif

#define WIDTH 960.0f
#define HEIGHT 540.0f
#define SPRITE_SCALE 5

#define GRAVITY 3600.0f

typedef struct Globals Globals;
struct Globals
{
  Arena perm_arena;

  Input input;
  Resources resources;
  R_Renderer renderer;
  Vec2F window;
  Vec4F viewport;
  bool debug;

  struct
  {
    f64 current_time;
    f64 elapsed_time;
    f64 accumulator;
  } frame;
};

// @Event ////////////////////////////////////////////////////////////////////////////////

typedef enum EventType
{
  EventType_EntityKilled,
} EventType;

typedef struct EventDesc EventDesc;
struct EventDesc
{
  u64 id;
  u64 type;
  b64 props;
};

typedef struct Event Event;
struct Event
{
  Event *next;
  EventType type;
  EventDesc desc;
};

typedef struct EventQueue EventQueue;
struct EventQueue
{
  Event *front;
  Event *back;
  u64 count;
};

void push_event(Game *game, EventType type, EventDesc desc);
void pop_event(Game *game);
Event *peek_event(Game *game);

// @Game /////////////////////////////////////////////////////////////////////////////////

typedef struct Game Game;
struct Game
{
  Arena frame_arena;
  Arena draw_arena;
  Arena entity_arena;

  Input *input;

  EventQueue event_queue;
  EntityList entities;
  Entity *entity_draw_list;

  f64 t;
  f64 dt;
  Mat3x3F camera;
  bool should_quit;

  Timer spawn_timer;
};

void init_game(Game *game);
void update_game(Game *game);
void render_game(Game *game);
bool game_should_quit(Game *game);

Vec2F screen_to_world(Vec2F pos);
