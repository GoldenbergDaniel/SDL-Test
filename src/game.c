#include <stdio.h>
#include <SDL2/SDL.h>

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
  GLOBAL->renderer->camera = &game->camera;

  // Create entities
  game->entity_head = create_player_entity(game);
  game->entity_tail = game->entity_head;
  game->entity_count = 1;

  Entity *entity = game->entity_head;
  for (u32 i = 0; i < 1; i++)
  {
    entity->next = create_enemy_entity(game);
    push_entity(game, entity->next);
    entity = entity->next;
  }
}

void update(Game *game)
{
  Entity *player = get_player(game);

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
        Entity *entity = create_laser_entity(game);
        entity->pos = event.descripter.position;
        entity->rot = event.descripter.rotation;
        entity->speed = event.descripter.speed;
        push_entity(game, entity);
      }
      break;
      default: break;
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

void handle_input(Game *game, SDL_Event *event)
{
  Input *input = GLOBAL->input;

  SDL_GetMouseState(&input->mouse_x, &input->mouse_y);

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

static
void push_entity(Game *game, Entity *entity)
{
  if (game->entity_tail != NULL)
  {
    game->entity_tail->next = entity;
  }

  entity->next = NULL;
  game->entity_tail = entity;
  game->entity_count++;

  printf("Entity count: %u\n", game->entity_count);
}
