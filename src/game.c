#include <stdio.h>
#include <SDL2/SDL.h>

#include "base/base_common.h"
#include "base/base_arena.h"
#include "draw.h"
#include "util.h"
#include "entity.h"
#include "game.h"

State *state;

void game_init(Game *game)
{
  state = arena_alloc(&game->arena, sizeof (State));
  state->input = arena_alloc(&game->arena, sizeof (Input));
  state->renderer = arena_alloc(&game->arena, sizeof (D_Renderer));
  d_init_renderer(state->renderer);

  // Create entities
  game->enemy_count = 5;
  game->entity_count = game->enemy_count + 1;
  game->entities[0] = create_player_entity();
  game->player = &game->entities[0];

  for (u8 i = 1; i < game->entity_count; i++)
  {
    game->entities[i] = create_enemy_entity(EntityType_Ship);
  }
}

void game_update(Game *game)
{
  Entity *entities = game->entities;
  Entity *player = game->player;

  for (u8 i = 0; i < game->entity_count; i++)
  {
    if (entities[i].flags & EntityFlag_Movable)
    {
      update_entity_movement(&entities[i], game->dt);
    }

    if (entities[i].flags & EntityFlag_Enemy)
    {
      if (player->active)
      {
        set_entity_target(&entities[i], player->pos);
      }
      else
      {
        set_entity_target(&entities[i], v2f(W_WIDTH / 2.0f, W_HEIGHT / 2.0f));
      }
    }
  }

  wrap_entity_at_edges(player);
}

void game_draw(Game *game)
{
  d_clear(v4f(0.05f, 0.05f, 0.05f, 1.0f));

  for (u8 i = 1; i < game->entity_count; i++)
  {
    Entity *entity = &game->entities[i];

    if (entity->active)
    {
      d_draw_rectangle(entity->xform, entity->color);
    }
  }

  if (game->player->active)
  {
    d_draw_triangle(game->player->xform, game->player->color);
  }
}

inline
bool game_should_quit(void)
{
  return state->input->escape;
}

void game_handle_event(Game *game, SDL_Event *event)
{
  Input *input = state->input;

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
