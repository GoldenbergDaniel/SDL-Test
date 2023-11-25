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

void init_game(Game *game)
{
  game->event_queue = (EventQueue) {0};
  game->camera = translate_3x3f(0.0f, 0.0f);
  game->should_quit = FALSE;

  SCOPE("Create starting entities")
  {
    Entity *entity = arena_alloc(&game->arena, sizeof (Entity));
    zero(*entity);
    GLOBAL->nil_entity = entity;

    entity = create_entity(game);
    init_entity(entity, EntityType_Wall);
    entity->pos = v2f(0.0f, 200);
    entity->color = COLOR_GRAY;
    set_entity_size(entity, W_WIDTH, 200);
    set_entity_origin(entity, v2i(-1, 1));

    entity = create_entity(game);
    init_entity(entity, EntityType_Player);

    // entity = create_entity(game);
    // init_entity(entity, EntityType_EnemyShip);
    
    entity = create_entity(game);
    init_entity(entity, EntityType_Wall);
    entity->color = COLOR_BLUE;

    entity = create_entity(game);
    init_entity(entity, EntityType_DebugLine);
    set_entity_size(entity, 100, 3);
    set_entity_origin(entity, v2i(-1, 0));

    Entity *player = get_nearest_entity_of_type(game, V2F_ZERO, EntityType_Player);
    set_entity_parent(entity, player);
    entity->pos.x = player->pos.x;
    entity->pos.y = player->pos.y;
    entity->local_pos.x = 15.0f;
  }
}

// @Update =====================================================================================

void update_game(Game *game)
{
  // Vec2I mouse_pos = get_mouse_position();
  // Vec2F player_pos = get_nearest_entity_of_type(game, V2F_ZERO, EntityType_Player)->pos;
  // printf("Mouse pos: %i, %i\n", mouse_pos.x, mouse_pos.y);
  // printf("Player pos: %f, %f\n", player_pos.x, player_pos.y);

  for (Entity *e = game->entities.head; e != NULL; e = e->next)
  {
    if (!e->is_active) continue;

    if (e->props & EntityProp_Collides)
    {
      update_entity_collider(e);
    }

    if (e->props & EntityProp_Movable)
    {
      if (e->props & EntityProp_Controlled)
      {
        update_controlled_entity_movement(game, e);
        wrap_entity_at_edges(e);
      }

      if (e->props & EntityProp_Autonomous)
      {
        update_autonomous_entity_movement(game, e);
      }
    }

    if (e->props & EntityProp_Rendered)
    {
      update_entity_xform(game, e);
    }

    if (e->props & EntityProp_Combatant)
    {
      if (e->props & EntityProp_Controlled)
      {
        update_controlled_entity_combat(game, e);
      }
      
      if (e->props & EntityProp_Autonomous && e->props & EntityProp_Hostile)
      {
        Entity *player = get_nearest_entity_of_type(game, e->pos, EntityType_Player);

        if (player && player->is_active)
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

  // DEBUG: Kill player on backspace
  if (key_just_pressed(KEY_BACKSPACE))
  {
    Entity *entity = get_nearest_entity_of_type(game, V2F_ZERO, EntityType_Player);
    EventDesc desc = {.id = entity->id};
    push_event(game, EventType_KillEntity, desc);
  }
}

// @HandleEvents ===============================================================================

void handle_game_events(Game *game)
{
  while (game->event_queue.count > 0)
  {
    Event event = get_next_event(game);

    switch (event.type)
    {
      case EventType_SpawnEntity:
      {
        Entity *entity = create_entity(game);

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
            printf("Entity ID: %llu\n", entity->id);
            printf("Entity Type: %i\n", entity->type);
            assert(FALSE);
          }
        }

        update_entity_xform(game, entity);
      }
      break;
      case EventType_KillEntity:
      {
        Entity *entity = get_entity_of_id(game, event.desc.id);
        destroy_entity(game, entity);
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
      default: 
      {
        printf("ERROR: Failed to process event. Invalid type!");
        assert(FALSE);
      }
    }

    pop_event(game);
  }
}

// @Draw =======================================================================================

void draw_game(Game *game)
{
  d_clear(COLOR_BLACK);

  for (Entity *e = game->entities.head; e != NULL; e = e->next)
  {
    if (!e->is_visible) continue;

    switch (e->type)
    {
      case EntityType_Player:
      {
        d_sprite(e->xform, e->color, 0);
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
bool game_should_quit(Game *game)
{
  bool result = FALSE;
  
  if (game->should_quit || key_pressed(KEY_ESCAPE))
  {
    result = TRUE;
  }

  return result;
}
