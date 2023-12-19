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
  zero(game->event_queue);
  game->camera = translate_3x3f(0.0f, 0.0f);
  game->projection = orthographic_3x3f(0.0f, WIDTH, 0.0f, HEIGHT);
  game->should_quit = FALSE;

  SCOPE("Create starting entities")
  {
    Entity *player = {0};
    
    Entity *en = arena_alloc(&game->perm_arena, sizeof (Entity));
    zero(*en);
    GLOBAL->nil_entity = en;

    en = create_entity(game);
    init_entity(en, EntityType_Wall);
    en->origin = v2f(0.0f, 1.0f);
    en->pos = v2f(WIDTH/2.0f, 200.0f);
    en->scale = v2f(120.0f, 20.0f);

    en = create_entity(game);
    init_entity(en, EntityType_Wall);
    en->color = D_RED;

    en = create_entity(game);
    init_entity(en, EntityType_Player);
    en->pos.y = 400;
    player = en;

    en = create_entity(game);
    init_entity(en, EntityType_Equipped);
    set_entity_parent(en, player);
    en->pos = v2f(30.0f, 1.0f);
    en->scale = v2f(1.0f, 1.0f);

    // en = create_entity(game);
    // init_entity(en, EntityType_EnemyShip);

    // Debug line
    #if 0
    {
      en = create_entity(game);
      init_entity(en, EntityType_DebugLine);
      set_entity_parent(en, player);
      en->origin = v2f(-1.0f, 0.0f);
      en->scale = v2f(3.0f, 0.05f);
      en->pos.x = 50.0f;
    }
    #endif
  }
}

// @Update =====================================================================================

void update_game(Game *game)
{
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

    if (en->props & EntityProp_Equipped)
    {
      update_equipped_entity(game, en);
    }
  }

  // DEBUG: Kill player on backspace
  {
    if (is_key_just_pressed(KEY_BACKSPACE))
    {
      Entity *player = get_nearest_entity_of_type(game, V2F_ZERO, EntityType_Player);
      kill_entity(game, .entity = player);
    }
  }

  // DEBUG: Switch player texture
  {
    Entity *en = get_nearest_entity_of_type(game, V2F_ZERO, EntityType_Player);
    if (is_key_just_pressed(KEY_1))
    {
      en->texture = D_TEXTURE_PLAYER;
    }
    else if (is_key_just_pressed(KEY_2))
    {
      en->texture = D_TEXTURE_ALIEN;
    }
    else if (is_key_just_pressed(KEY_3))
    {
      en->texture = D_TEXTURE_GUN;
    }
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
  d_clear(D_BLACK);

  for (Entity *en = game->entities.head; en; en = en->next)
  {
    if (!en->visible) continue;

    switch (en->type)
    {
      case EntityType_Player:
      case EntityType_Equipped:
      { 
        d_sprite(en->xform, en->color, en->texture);
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
bool game_should_quit(Game *game)
{
  bool result = FALSE;
  
  if (game->should_quit || is_key_pressed(KEY_ESCAPE))
  {
    result = TRUE;
  }

  return result;
}

Entity *_spawn_entity(Game *game, EntityType type, EntitySpawnParams params)
{
  Entity *en = create_entity(game);
  init_entity(en, type);
  en->props = en->props | params.props;
  en->pos = params.pos;
  en->color = params.color;

  return en;
}

void _kill_entity(Game *game, EntityKillParams params)
{
  Entity *en = params.entity;
  if (en == NULL)
  {
    en = get_entity_of_id(game, params.id);
  }

  destroy_entity(game, en);
}

Vec2F screen_to_world(Vec2F pos)
{
  Vec3F result = v3f(pos.x, pos.y, 1);
  // result = transform_3f(result, scale_3x3f(1.0f, 1.0f/HEIGHT));
  result.y = HEIGHT - pos.y; 

  return v2f(result.x, result.y);
}
