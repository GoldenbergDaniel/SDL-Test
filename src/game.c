#include <stdio.h>
#include <math.h>

#include "base/base_common.h"
#include "base/base_arena.h"
#include "base/base_math.h"

#include "gfx/draw.h"
#include "physx/physx.h"
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
    Entity *en = arena_alloc(&game->perm_arena, sizeof (Entity));
    zero(*en);
    GLOBAL->nil_entity = en;

    en = create_entity(game);
    init_entity(en, EntityType_Wall);
    f32 wall_y = 200.0f;
    en->pos = v2f(0.0f, wall_y);
    en->color = COLOR_GRAY;
    set_entity_size(en, W_WIDTH, wall_y);
    set_entity_origin(en, v2i(-1, 1));

    en = create_entity(game);
    init_entity(en, EntityType_Player);
    en->pos.y = 240;

    // en = create_entity(game);
    // init_entity(en, EntityType_EnemyShip);
    
    en = create_entity(game);
    init_entity(en, EntityType_Wall);
    en->color = COLOR_BLUE;

    en = create_entity(game);
    init_entity(en, EntityType_DebugLine);
    set_entity_size(en, 100, 1);
    set_entity_origin(en, v2i(-1, 0));

    Entity *player = get_nearest_entity_of_type(game, V2F_ZERO, EntityType_Player);
    set_entity_parent(en, player);
    en->pos.x = player->pos.x;
    en->pos.y = player->pos.y;
    en->local_pos.x = 15.0f;
  }
}

// @Update =====================================================================================

void update_game(Game *game)
{
  // Vec2I mouse_pos = get_mouse_position();
  // Vec2F player_pos = get_nearest_entity_of_type(game, V2F_ZERO, EntityType_Player)->pos;
  // printf("MOUSE POS: %i, %i\n", mouse_pos.x, mouse_pos.y);
  // printf("Player pos: %f, %f\n", player_pos.x, player_pos.y);

  for (Entity *en = game->entities.head; en; en = en->next)
  {
    if (!en->active) continue;

    if (en->props & EntityProp_Collides)
    {
      update_entity_collider(en);
    }

    if (en->props & EntityProp_Movable)
    {
      if (en->props & EntityProp_Controlled)
      {
        update_controlled_entity_movement(game, en);
        wrap_entity_at_edges(en);
        
        if (en->type == EntityType_Player)
        {
          Entity *wall = get_nearest_entity_of_type(game, V2F_ZERO, EntityType_Wall);
          resolve_entity_collision(en, wall);
        }
      }

      if (en->props & EntityProp_Autonomous)
      {
        update_autonomous_entity_movement(game, en);
      }
    }

    if (en->props & EntityProp_Rendered)
    {
      update_entity_xform(game, en);
    }

    if (en->props & EntityProp_Combatant)
    {
      if (en->props & EntityProp_Controlled)
      {
        update_controlled_entity_combat(game, en);
      }
      
      if (en->props & EntityProp_Autonomous && en->props & EntityProp_Hostile)
      {
        Entity *player = get_nearest_entity_of_type(game, en->pos, EntityType_Player);

        if (player && player->active)
        {
          set_entity_target(en, ref_from_entity(player));
        }
        else
        {
          en->has_target = FALSE;
        }

        update_targetting_entity_combat(game, en);
      }
    }
}

  // DEBUG: Kill player on backspace
  if (is_key_just_pressed(KEY_BACKSPACE))
  {
    Entity *en = get_nearest_entity_of_type(game, V2F_ZERO, EntityType_Player);
    EventDesc desc = {.id = en->id};
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
        Entity *en = create_entity(game);

        switch (event.desc.type)
        {
          case EntityType_EnemyShip:
          {
            init_entity(en, event.desc.type);
          }
          break;
          case EntityType_Laser:
          {
            init_entity(en, event.desc.type);
            en->pos = event.desc.position;
            en->rot = event.desc.rotation;
            en->speed = event.desc.speed;
            en->color = event.desc.color;
          }
          break;
          case EntityType_Wall:
          {
            init_entity(en, event.desc.type);
            en->pos = event.desc.position;
            en->scale = v2f(0.1, 0.1);
          }
          break;
          default: 
          {
            printf("ERROR: Failed to spawn en. Invalid type!");
            printf("Entity ID: %llu\n", en->id);
            printf("Entity Type: %i\n", en->type);
            assert(FALSE);
          }
        }

        update_entity_xform(game, en);
      }
      break;
      case EventType_KillEntity:
      {
        Entity *en = get_entity_by_id(game, event.desc.id);
        destroy_entity(game, en);
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

  sort_entities_by_z_index(game);

  for (Entity *en = game->entities.head; en; en = en->next)
  {
    if (!en->visible) continue;

    switch (en->type)
    {
      case EntityType_Player:
      {
        d_sprite(en->xform, en->color, 0);
      }
      break;
      case EntityType_EnemyShip:
      {
        d_triangle(en->xform, en->color);
      }
      break;
      case EntityType_Wall:
      case EntityType_Laser:
      case EntityType_DebugLine:
      {
        d_rectangle(en->xform, en->color);
      }
      break;
      default: break;
    }
  }
}

inline
void clear_game_frame_arena(Game *game)
{
  clear_arena(&game->frame_arena);
}

inline
bool game_should_quit(Game *game)
{
  bool result = FALSE;
  
  if (game->should_quit || is_key_pressed(KEY_ESCAPE))
  {
    result = TRUE;
  }

  return result;
}
