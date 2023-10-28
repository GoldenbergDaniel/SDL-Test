#pragma once

#include "base/base_common.h"
#include "base/base_math.h"

typedef struct Game Game;

typedef enum EventType
{
  EventType_SpawnEntity,
  EventType_KillEntity,
} EventType;

typedef struct EventDescriptor EventDescriptor;
struct EventDescriptor
{
  u64 id;
  u64 type;
  b64 props;
  Vec2F position;
  f32 rotation;
  f32 speed;
};

typedef struct Event Event;
struct Event
{
  Event *next;
  EventType type;
  EventDescriptor descriptor;
};

typedef struct EventQueue EventQueue;
struct EventQueue
{
  Event *front;
  Event *back;
  u64 count;
};

void push_event(Game *game, EventType type, EventDescriptor desc);
void pop_event(Game *game);
Event peek_event(Game *game);
void clear_event_queue(Game *game);
