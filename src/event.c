#include "base/base_inc.h"

#include "game.h"
#include "event.h"

void push_event(Game *game, EventType type, EventDesc desc)      
{
  EventQueue *queue = &game->event_queue;
  Event *new_event = queue->first_free;

  if (new_event == NULL)
  {
    new_event = arena_alloc(&game->frame_arena, sizeof (Event));
    zero(*new_event, Event);
  }
  else
  {
    queue->first_free = queue->first_free->next_free;
  }

  if (queue->front == NULL)
  {
    queue->front = new_event;
  }

  if (queue->back != NULL)
  {
    queue->back->next = new_event;
  }

  new_event->type = type;
  new_event->desc = desc;
  new_event->next = NULL;

  queue->back = new_event;
  queue->count++;
}

void pop_event(Game *game)
{
  assert(game->event_queue.count > 0);

  EventQueue *queue = &game->event_queue;
  Event *next = queue->front->next;
  zero(*queue->front, Event);
  
  if (queue->count == 2)
  {
    queue->back = next;
  }

  queue->front = next;
  queue->count--;
}

inline
Event *peek_event(Game *game)
{
  return game->event_queue.front;
}

void clear_event_queue(Game *game)
{
  EventQueue *queue = &game->event_queue;
  Event *curr = queue->front;

  while (curr != NULL)
  {
    Event *next = curr->next;
    zero(*curr, Event);
    curr = next;
  }

  zero(*queue, EventQueue);
}
