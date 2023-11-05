#pragma once

#include "base/base_common.h"
#include "base/base_math.h"

typedef struct Game Game;

typedef struct Event Event;
typedef struct EventDesc EventDesc;
typedef struct EventQueue EventQueue;

typedef enum EventType
{
  EventType_SpawnEntity,
  EventType_KillEntity,
  EventType_EntityKilled,
} EventType;

struct EventDesc
{
  u64 id;
  u64 type;
  b64 props;

  union
  {
    // Entity
    struct
    {
      Vec2F position;
      f32 rotation;
      Vec2F scale;
      f32 speed;
      Vec4F color;
    };
  };
};

struct Event
{
  Event *next;
  EventType type;
  EventDesc desc;
};

struct EventQueue
{
  Event *front;
  Event *back;
  u64 count;
};

void push_event(Game *game, EventType type, EventDesc desc);
void pop_event(Game *game);
Event get_next_event(Game *game);
void clear_event_queue(Game *game);
