#include <stdio.h>

#include "base/base_common.h"
#include "base/base_arena.h"
#include "base/base_math.h"

#include "gfx/draw.h"
#include "input.h"
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
  game->first_frame = TRUE;
  game->should_quit = FALSE;

  Entity *entity;
  entity = alloc_entity(game);
  init_entity(entity, EntityType_Player);
  entity = alloc_entity(game);
  init_entity(entity, EntityType_EnemyShip);

  GLOBAL->renderer->camera = &game->camera;
}

// @Update =====================================================================================

void update(Game *game)
{
  for (Entity *e = game->entities.head; e != NULL; e = e->next)
  {
    if (!e->is_active) continue;

    if (e->props & EntityProp_Movable)
    {
      if (e->props & EntityProp_Controlled)
      {
        update_controlled_entity_movement(game, e);
        wrap_entity_at_edges(e);
      }

      if (e->props & EntityProp_Targetting)
      {
        update_targetting_entity_movement(game, e);
      }

      if (e->props & EntityProp_Projectile)
      {
        update_projectile_entity_movement(game, e);
      }

      update_entity_xform(game, e);
    }

    if (e->props & EntityProp_Attacker)
    {
      if (e->props & EntityProp_Controlled)
      {
        update_controlled_entity_combat(game, e);
      }
      
      if (e->props & EntityProp_Targetting)
      {
        Entity *player = get_nearest_entity_of_type(game, e->pos, EntityType_Player);

        if (player != NULL && player->is_active)
        {
          set_entity_target(e, ref_from_entity(player));
        }
        else
        {
          e->has_target = FALSE;
        }

        update_targetting_entity_combat(game, e);
      }
    }
  }

  if (key_just_pressed(KEY_BACKSPACE))
  {
    Entity *entity = get_nearest_entity_of_type(game, V2F_ZERO, EntityType_Player);
    EventDesc desc = {.id = entity->id};
    push_event(game, EventType_KillEntity, desc);
  }
}

// @HandleEvents ===============================================================================

void handle_events(Game *game)
{
  while (game->event_queue.count > 0)
  {
    Event event = get_next_event(game);

    switch (event.type)
    {
      case EventType_SpawnEntity:
      {
        Entity *entity = alloc_entity(game);

        switch (event.descriptor.type)
        {
          case EntityType_EnemyShip:
          {
            init_entity(entity, event.descriptor.type);
          }
          break;
          case EntityType_Laser:
          {
            init_entity(entity, event.descriptor.type);
            entity->pos = event.descriptor.position;
            entity->rot = event.descriptor.rotation;
            entity->speed = event.descriptor.speed;
            entity->color = event.descriptor.color;
          }
          break;
          default: 
          {
            printf("ERROR: Failed to spawn entity. Invalid type!");
            ASSERT(FALSE);
          }
        }
      }
      break;
      case EventType_KillEntity:
      {
        Entity *entity = get_entity_of_id(game, event.descriptor.id);
        free_entity(game, entity);
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
    if (!e->is_visible) continue;

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
  
  if (game->should_quit || key_pressed(KEY_ESCAPE))
  {
    result = TRUE;
  }

  return result;
}
