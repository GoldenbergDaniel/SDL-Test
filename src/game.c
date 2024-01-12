#include <stdio.h>
#include <math.h>

#include "base/base_common.h"
#include "base/base_arena.h"
#include "base/base_math.h"

#include "gfx/draw.h"
#include "input.h"
#include "event.h"
#include "entity.h"
#include "global.h"
#include "game.h"

extern Global *GLOBAL;

// @Init =======================================================================================

void init_game(Game *game)
{
  game->camera = translate_3x3f(0.0f, 0.0f);
  game->projection = orthographic_3x3f(0.0f, WIDTH, 0.0f, HEIGHT);

  SCOPE("Starting entities")
  {
    Entity *ground = alloc_entity(game);
    init_entity(ground, EntityType_Wall);
    ground->origin = v2f(0.0f, 1.0f);
    ground->pos = v2f(WIDTH/2.0f, 200.0f);
    ground->scale = v2f(120.0f, 20.0f);
    ground->color = v4f(0.7f, 0.6f, 0.4f, 1.0f);

    Entity *player = alloc_entity(game);
    init_entity(player, EntityType_Player);
    player->pos = v2f(WIDTH/2.0f, HEIGHT/2.0f + 50.0f);
    player->scale = v2f(8.0f, 8.0f);

    Entity *gun = alloc_entity(game);
    init_entity(gun, EntityType_Equipped);
    set_entity_parent(gun, player);
    gun->pos = v2f(30.0f, 0.0f);
    gun->scale = v2f(1.0f, 1.0f);

    Entity *shot_point = alloc_entity(game);
    init_entity(shot_point, EntityType_Debug);
    set_entity_parent(shot_point, gun);
    shot_point->pos = v2f(24.0f, 2.0f);
    shot_point->scale = v2f(0.05f, 0.05f);
    shot_point->visible = FALSE;

    Entity *zombie = alloc_entity(game);
    init_entity(zombie, EntityType_ZombieWalker);
    zombie->pos = v2f(WIDTH - 300.0f, HEIGHT/2.0f + 50.0f);
    zombie->scale = v2f(8.0f, 8.0f);
  }
}

// @Update =====================================================================================

void update_game(Game *game)
{
  Entity *player = get_first_entity_of_type(game, EntityType_Player);

  for (Entity *en = game->entities.head; en; en = en->next)
  {
    if (!en->active) continue;

    if (en->props & EntityProp_Moves && game->t > 0.35f)
    {
      if (en->props & EntityProp_Controlled)
      {
        update_controlled_entity_movement(game, en);
        wrap_entity_at_edges(en);
      }

      if (en->props & EntityProp_Autonomous)
      {
        update_autonomous_entity_movement(game, en);
      }

      if (en->props & EntityProp_Collides)
      {
        update_entity_collider(en);
        Entity *ground = get_first_entity_of_type(game, EntityType_Wall);
        resolve_entity_collision(en, ground);
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
      
      if (en->props & EntityProp_Autonomous)
      {
        if (is_entity_valid(player) && player->active)
        {
          set_entity_target(en, ref_from_entity(player));
        }
        else
        {
          en->has_target = FALSE;
        }

        if (en->has_target)
        {
          update_targetting_entity_combat(game, en);
        }
      }
    }

    if (en->props & EntityProp_Equipped)
    {
      update_equipped_entity(game, en);
    }
  }

  // DEBUG: Kill player on backspace press
  if (is_key_just_pressed(KEY_BACKSPACE))
  {
    kill_entity(game, .entity = player);
  }

  // DEBUG: Switch player texture
  if (is_key_just_pressed(KEY_1))
  {
    player->texture = D_SPRITE_COWBOY;
  }
  else if (is_key_just_pressed(KEY_2))
  {
    player->texture = D_SPRITE_ZOMBIE;
  }
  else if (is_key_just_pressed(KEY_3))
  {
    player->texture = D_SPRITE_GUN;
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
  d_clear(v4f(0.5f, 0.32f, 0.32f, 1.0f));

  SCOPE("Primitive Batch")
  {
    for (Entity *en = game->entities.head; en; en = en->next)
    {
      if (en->draw_type == DrawType_Rectangle && en->visible)
      {
        d_draw_rectangle(en->xform, en->color);
      }
    }
  }

  SCOPE("Sprite Batch")
  {
    for (Entity *en = game->entities.head; en; en = en->next)
    {
      if (en->draw_type == DrawType_Sprite && en->visible)
      {
        d_draw_sprite(en->xform, en->color, en->texture);
      }
    }
  }

  r_flush(&GLOBAL->renderer);
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
