#include <stdio.h>
#include <SDL2/SDL.h>

#include "base/base_common.h"
#include "base/base_arena.h"
#include "base/base_math.h"
#include "gfx/draw.h"
#include "global.h"
#include "component.h"
#include "entity.h"
#include "game.h"

extern Global *GLOBAL;

void init(Game *game)
{
  game->camera = translate_3x3f(0.0f, 0.0f);
  GLOBAL->renderer->camera = &game->camera;

  // Create entities
  game->entity_head = create_player_entity(game);
  game->entity_tail = game->entity_head;
  game->entity_count = 1;

  Entity *entity = game->entity_head;
  for (u32 i = 0; i < 3; i++)
  {
    entity->next = create_enemy_entity(game, EntityClass_Ship);
    game->entity_tail = entity->next;
    game->entity_count++;
    entity = entity->next;
  }
}

void update(Game *game)
{
  Entity *player = get_player(game);

  for (Entity *e = game->entity_head; e != NULL; e = e->next)
  {
    if (e->props & EntityProp_Movable)
    {
      if (e->props & EntityProp_Controlled)
      {
        update_controlled_entity_movement(game, e);
        wrap_entity_at_edges(game, e);
      }

      if (e->props & EntityProp_Targetting)
      {
        update_targetting_entity_movement(game, e);
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

void draw(Game *game)
{
  d_clear(v4f(0.05f, 0.05f, 0.05f, 1.0f));

  for (Entity *e = game->entity_head; e != NULL; e = e->next)
  {
    if (e->active)
    {
      switch (e->class)
      {
        case EntityClass_Ship:
        {
          d_triangle(e->xform, e->color);
        }
        break;
        case EntityClass_Astreroid:
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
bool should_quit(void)
{
  return GLOBAL->input->escape;
}

Entity *get_player(Game *game)
{
  Entity *result = NULL;

  for (Entity *e = game->entity_head; e != NULL; e = e->next)
  {
    if (e->props & EntityProp_Controlled)
    {
      result = e;
    }
  }

  return result;
}

void handle_event(Game *game, SDL_Event *event)
{
  Input *input = GLOBAL->input;

  switch (event->type)
  {
    case SDL_QUIT: game->running = FALSE;
    break;
    case SDL_KEYDOWN: 
    {
      switch (event->key.keysym.scancode)
      {
        default: break;
        case SDL_SCANCODE_ESCAPE: input->escape = TRUE;
        break;
        case SDL_SCANCODE_SPACE: input->space = TRUE;
        break;
        case SDL_SCANCODE_A: input->a = TRUE;
        break;
        case SDL_SCANCODE_D: input->d = TRUE;
        break;
        case SDL_SCANCODE_S: input->s = TRUE;
        break;
        case SDL_SCANCODE_W: input->w = TRUE;
        break;
      }
      break;
    }
    case SDL_KEYUP: 
    {
      switch (event->key.keysym.scancode)
      {
        default: break;
        case SDL_SCANCODE_ESCAPE: input->escape = FALSE;
        break;
        case SDL_SCANCODE_SPACE: input->space = FALSE;
        break;
        case SDL_SCANCODE_A: input->a = FALSE;
        break;
        case SDL_SCANCODE_D: input->d = FALSE;
        break;
        case SDL_SCANCODE_S: input->s = FALSE;
        break;
        case SDL_SCANCODE_W: input->w = FALSE;
        break;
      }
      break;
    }
  }
}
