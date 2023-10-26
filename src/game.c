#include <stdio.h>

#include "base/base_common.h"
#include "base/base_arena.h"
#include "base/base_math.h"

#include "gfx/draw.h"
#include "global.h"
#include "component.h"
#include "event.h"
#include "entity.h"
#include "game.h"

extern Global *GLOBAL;

static void push_entity(Game *game, Entity *entity);

void init(Game *game)
{
  game->event_queue = (EventQueue) {0};
  game->camera = translate_3x3f(0.0f, 0.0f);
  game->running = TRUE;
  game->should_quit = FALSE;
  game->first_frame = TRUE;

  Entity *entity = create_player_entity(game);
  push_entity(game, entity);

  entity = game->entity_head;
  for (u32 i = 0; i < 3; i++)
  {
    entity->next = create_enemy_entity(game);
    push_entity(game, entity->next);
    entity = entity->next;
  }

  GLOBAL->renderer->camera = &game->camera;
}

void update(Game *game)
{
  for (Entity *e = game->entity_head; e != NULL; e = e->next)
  {
    b64 props = e->props;

    if (props & EntityProp_Movable)
    {
      if (props & EntityProp_Controlled)
      {
        update_controlled_entity_movement(game, e);
        wrap_entity_at_edges(e);
      }

      if (props & EntityProp_Targetting)
      {
        update_targetting_entity_movement(game, e);
      }

      if (props & EntityProp_Projectile)
      {
        update_projectile_entity_movement(game, e);
      }

      update_entity_xform(game, e);
    }

    if (props & EntityProp_Attacker)
    {
      if (props & EntityProp_Controlled)
      {
        update_controlled_entity_combat(game, e);
      }
      
      if (props & EntityProp_Targetting)
      {
        Entity *player = get_first_entity_of_type(game, EntityType_Player);

        update_targetting_entity_combat(game, e);

        if (player->active)
        {
          set_entity_target(game, e, player);
        }
        else
        {
          set_entity_target(game, e, e);
        }
      }
    }
  }
}

void handle_events(Game *game)
{
  EventQueue *queue = &game->event_queue;

  while (queue->count > 0)
  {
    Event event = peek_event(queue);

    switch (event.type)
    {
      case EventType_SpawnEntity:
      {
        Entity *entity = NULL;

        switch (event.descripter.type)
        {
          case EntityType_EnemyShip:
          {
            entity = create_enemy_entity(game);
            entity->pos = event.descripter.position;
            entity->rot = event.descripter.rotation;
          }
          break;
          case EntityType_Laser:
          {
            entity = create_laser_entity(game);
            entity->pos = event.descripter.position;
            entity->rot = event.descripter.rotation;
            entity->speed = event.descripter.speed;
          }
          break;
          default: ASSERT(FALSE);
        }

        push_entity(game, entity);
      }
      break;
      case EventType_KillEntity:
      {
        // Kill entity
      }
      break;
    }

    pop_event(queue);
  }
}

void draw(Game *game)
{
  d_clear(COLOR_BLACK);

  for (Entity *e = game->entity_head; e != NULL; e = e->next)
  {
    if (e->active)
    {
      switch (e->type)
      {
        case EntityType_Player:
        {
          d_triangle(e->xform, e->color);
        }
        break;
        case EntityType_EnemyShip:
        {
          d_triangle(e->xform, e->color);
        }
        break;
        case EntityType_Laser:
        {
          d_rectangle(e->xform, e->color);
        }
        break;
        default: break;
      }
    }
  }
}

inline
bool should_quit(Game *game)
{
  bool result = FALSE;
  
  if (key_pressed(KEY_ESCAPE))
  {
    result = TRUE;
  }

  return result;
}

Entity *get_first_entity_of_type(Game *game, EntityType type)
{
  Entity *result = NULL;

  for (Entity *e = game->entity_head; e != NULL; e = e->next)
  {
    if (e->type == type)
    {
      result = e;
      break;
    }
  }

  ASSERT(result != NULL);

  return result;
}

static
void push_entity(Game *game, Entity *entity)
{
  if (game->entity_head == NULL)
  {
    game->entity_head = entity;
  }

  if (game->entity_tail != NULL)
  {
    game->entity_tail->next = entity;
  }

  entity->next = NULL;
  game->entity_tail = entity;
  game->entity_count++;

  printf("Entity count: %u\n", game->entity_count);
}
