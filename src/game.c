#include <stdio.h>
#include <math.h>

#include "base/base_common.h"
#include "base/base_arena.h"
#include "base/base_math.h"

#include "gfx/draw.h"
#include "phys/phys.h"
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
  game->is_running = TRUE;
  game->should_quit = FALSE;

  GLOBAL->renderer->camera = &game->camera;

  Entity *entity;
  entity = alloc_entity(game);
  init_entity(entity, EntityType_Wall);
  entity->pos = v2f(0.0f, 200);
  entity->color = COLOR_GRAY;
  set_entity_size(entity, W_WIDTH, 200);
  set_entity_origin(entity, v2i(-1, 1));

  entity = alloc_entity(game);
  init_entity(entity, EntityType_Player);
  // entity = alloc_entity(game);
  // init_entity(entity, EntityType_EnemyShip);
  entity = alloc_entity(game);
  init_entity(entity, EntityType_Wall);

  entity = alloc_entity(game);
  init_entity(entity, EntityType_DebugLine);
  set_entity_size(entity, 100, 3);
  set_entity_origin(entity, v2i(-1, 0));

  // printf("%lu\n", sizeof (Entity));
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
    }

    if (e->props & EntityProp_Rendered)
    {
      update_entity_xform(e);
    }

    if (e->props & EntityProp_Attacker)
    {
      if (e->props & EntityProp_Controlled)
      {
        update_controlled_entity_combat(game, e);
      }
      
      if (e->props & EntityProp_Targetting)
      {
        EntityRef player = get_nearest_entity_of_type(game, e->pos, EntityType_Player);

        if (entity_from_ref(player) && player.ptr->is_active)
        {
          set_entity_target(e, player);
        }
        else
        {
          e->has_target = FALSE;
        }

        update_targetting_entity_combat(game, e);
      }
    }
  }

  EntityRef player = get_nearest_entity_of_type(game, V2F_ZERO, EntityType_Player);
  EntityRef line = get_nearest_entity_of_type(game, V2F_ZERO, EntityType_DebugLine);

  if (entity_from_ref(player) && entity_from_ref(line))
  {
    line.ptr->pos.x = player.ptr->pos.x;
    line.ptr->pos.y = player.ptr->pos.y + 10;
    line.ptr->rot = player.ptr->rot;

    // if (p_polygon_y_range_intersect(&player.ptr->col, v2f(0, 200), v2f(W_WIDTH, 200)))
    // {
    //   printf("collided!\n");
    // }
  }

  if (key_just_pressed(KEY_BACKSPACE))
  {
    EntityRef entity = get_nearest_entity_of_type(game, V2F_ZERO, EntityType_Player);
    EventDesc desc = {.id = entity.id};
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

        switch (event.desc.type)
        {
          case EntityType_EnemyShip:
          {
            init_entity(entity, event.desc.type);
          }
          break;
          case EntityType_Laser:
          {
            init_entity(entity, event.desc.type);
            entity->pos = event.desc.position;
            entity->rot = event.desc.rotation;
            entity->speed = event.desc.speed;
            entity->color = event.desc.color;
          }
          break;
          case EntityType_Wall:
          {
            init_entity(entity, event.desc.type);
            entity->pos = event.desc.position;
            entity->scale = v2f(0.1, 0.1);
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
        EntityRef entity = get_entity_of_id(game, event.desc.id);
        free_entity(game, entity.ptr);
      }
      break;
      case EventType_EntityKilled:
      {
        EntityType type = event.desc.type;

        if (type == EntityType_Player)
        {
          // game_over = TRUE
        }
      }
      default: break;
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
      case EntityType_Wall:
      {
        d_rectangle(e->xform, e->color);
      }
      break;
      case EntityType_DebugLine:
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
