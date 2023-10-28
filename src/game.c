#include <stdio.h>

#include "base/base_common.h"
#include "base/base_arena.h"
#include "base/base_math.h"

#include "gfx/draw.h"
#include "global.h"
#include "event.h"
#include "entity.h"
#include "game.h"

extern Global *GLOBAL;

// @Init =======================================================================================

void init(Game *game)
{
  game->event_queue = (EventQueue) {0};
  game->camera = translate_3x3f(0.0f, 0.0f);
  game->running = TRUE;
  game->should_quit = FALSE;
  game->first_frame = TRUE;

  Entity *entity = create_player_entity(game);
  push_entity(game, entity);

  entity = game->entities.head;
  for (u32 i = 0; i < 1; i++)
  {
    entity->next = create_enemy_entity(game);
    push_entity(game, entity->next);
    entity = entity->next;
  }

  GLOBAL->renderer->camera = &game->camera;
}

// @Update =====================================================================================

void update(Game *game)
{
  for (Entity *e = game->entities.head; e != NULL; e = e->next)
  {
    if (!e->active) continue;
    
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
        update_targetting_entity_combat(game, e);

        Entity *player = get_first_entity_of_type(game, EntityType_Player);

        if (player != NULL && player->active)
        {
          set_entity_target(game, e, entity_ref(player));
        }
        else
        {
          e->has_target = FALSE;
        }
      }
    }
  }

  if (key_just_pressed(KEY_BACKSPACE))
  {
    Entity *entity = get_first_entity_of_type(game, EntityType_Player);
    EventDescriptor desc = {.id = entity->id};
    push_event(game, EventType_KillEntity, desc);
  }
}

// @HandleEvents ===============================================================================

void handle_events(Game *game)
{
  while (game->event_queue.count > 0)
  {
    Event event = peek_event(game);

    switch (event.type)
    {
      case EventType_SpawnEntity:
      {
        Entity *entity = NULL;

        switch (event.descriptor.type)
        {
          case EntityType_EnemyShip:
          {
            entity = create_enemy_entity(game);
            entity->pos = event.descriptor.position;
            entity->rot = event.descriptor.rotation;
          }
          break;
          case EntityType_Laser:
          {
            entity = create_laser_entity(game);
            entity->pos = event.descriptor.position;
            entity->rot = event.descriptor.rotation;
            entity->speed = event.descriptor.speed;
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
        pop_entity(game, event.descriptor.id);
      }
      break;
    }

    pop_event(game);
  }
}

// @Draw =======================================================================================

void draw(Game *game)
{
  d_clear(COLOR_BLACK);

  for (Entity *e = game->entities.head; e != NULL; e = e->next)
  {
    if (!e->active) continue;

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
