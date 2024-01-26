#pragma once

#include "base/base_inc.h"

typedef struct Game Game;

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

void push_event(Game *game, EventType type, EventDesc desc);
Event *peek_event(Game *game);
void pop_event(Game *game);
void clear_event_queue(Game *game);
