#pragma once

#include "base/base_inc.h"

#include "entity.h"

// @Event ///////////////////////////////////////////////////////////////////////////

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
  Event *next_free;
  
  EventType type;
  EventDesc desc;
};

typedef struct EventQueue EventQueue;
struct EventQueue
{
  Event *front;
  Event *back;
  Event *first_free;
  u64 count;
};

// @Game ////////////////////////////////////////////////////////////////////////////

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
  
  bool is_sim_started;
  bool should_quit;
};

void init_game(Game *game);
void update_game(Game *game);
void handle_game_events(Game *game);
void draw_game(Game *game, Game *prev);
bool game_should_quit(Game *game);
void copy_game_state(Game *game, Game *prev);
