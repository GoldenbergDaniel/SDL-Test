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

  SCOPE("Create starting entities")
  {
    Entity *ground = alloc_entity(game);
    init_entity(ground, EntityType_Wall);
    ground->origin = v2f(0.0f, 1.0f);
    ground->pos = v2f(WIDTH/2.0f, 200.0f);
    ground->scale = v2f(120.0f, 20.0f);
    ground->color = D_GRAY;

    Entity *player = alloc_entity(game);
    init_entity(player, EntityType_Player);
    player->pos.y = 400;

    Entity *gun = alloc_entity(game);
    init_entity(gun, EntityType_Equipped);
    set_entity_parent(gun, player);
    gun->pos = v2f(30.0f, 1.0f);
    gun->scale = v2f(1.0f, 1.0f);

    Entity *shot_point = alloc_entity(game);
    init_entity(shot_point, EntityType_Debug);
    set_entity_parent(shot_point, gun);
    shot_point->pos = v2f(24.0f, 2.0f);
    shot_point->scale = v2f(0.05f, 0.05f);
    shot_point->visible = FALSE;
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
          Entity *wall = get_first_entity_of_type(game, EntityType_Wall);
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
        Entity *player = get_first_entity_of_type(game, EntityType_Player);

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
      Entity *player = get_first_entity_of_type(game, EntityType_Player);
      kill_entity(game, .entity = player);
    }
  }

  // DEBUG: Switch player texture
  {
    Entity *en = get_first_entity_of_type(game, EntityType_Player);
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
        switch (event.desc.type)
        {
          case EntityType_Player:
          {
            // game_over = TRUE
          }
          break;
        }
      }
      default: 
      {
        fprintf(stderr, "ERROR: Failed to process event. Invalid type!");
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

    switch (en->draw_type)
    {
      case DrawType_Sprite:
      { 
        d_sprite(en->xform, en->color, en->texture);
      }
      break;
      case DrawType_Triangle:
      {
        d_triangle(en->xform, en->color);
      }
      break;
      case DrawType_Rectangle:
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

Vec2F screen_to_world(Vec2F pos)
{
  Vec3F result = v3f(pos.x, pos.y, 1);
  result.y = HEIGHT - pos.y; 

  return v2f(result.x, result.y);
}
