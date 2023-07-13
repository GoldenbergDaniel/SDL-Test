#include <SDL2/SDL.h>

#include "globals.h"
#include "util.h"
#include "draw.h"
#include "input.h"
#include "entity.h"
#include "game.h"

Input *input;

// NOTE: Just no
static f32 max_delay = 1.0f;
static f32 cur_delay = 0.0f;
const f32 dlwq = 0.0f;

void game_init(Game *game)
{
  input = (Input *) malloc(sizeof (Input *));

  // Create entities
  game->enemy_count = 1;
  game->entity_count = game->enemy_count++;
  game->entities[0] = entity_create(ENTITY_PLAYER);
  game->player = &game->entities[0];

  for (u8 i = 1; i < game->entity_count; i++)
  {
    game->entities[i] = entity_create(ENTITY_ENEMY);
  }

  // Set starting positions
  for (u8 i = 0; i < game->entity_count; i++)
  {
    entity_start(&game->entities[i]);
  }
}

void game_handle_event(Game *game, SDL_Event *event)
{   
  switch (event->type)
  {
    case SDL_QUIT: { game->is_running = FALSE; } break;
    case SDL_KEYDOWN: 
    {
      switch (event->key.keysym.scancode)
      {
        default: break;
        case SDL_SCANCODE_ESCAPE: { input->escape = TRUE; } break;
        case SDL_SCANCODE_SPACE: { input->space = TRUE; } break;
        case SDL_SCANCODE_A: { input->a = TRUE; } break;
        case SDL_SCANCODE_D: { input->d = TRUE; } break;
        case SDL_SCANCODE_S: { input->s = TRUE; } break;
        case SDL_SCANCODE_W: { input->w = TRUE; } break;
        break;
      }
      break;
    }
    case SDL_KEYUP: 
    {
      switch (event->key.keysym.scancode)
      {
        default: break;
        case SDL_SCANCODE_ESCAPE: { input->escape = FALSE; } break;
        case SDL_SCANCODE_SPACE: { input->space = FALSE; } break;
        case SDL_SCANCODE_A: { input->a = FALSE; } break;
        case SDL_SCANCODE_D: { input->d = FALSE; } break;
        case SDL_SCANCODE_S: { input->s = FALSE; } break;
        case SDL_SCANCODE_W: { input->w = FALSE; } break;
        break;
      }
      break;
    }
  }
}

void game_update(Game *game)
{
  Entity *player = game->player;
  Entity *entities = game->entities;

  // Update entities
  entity_update(player, game->t, game->dt);
  
  for (u8 i = 1; i < game->entity_count; i++)
  {
    if (player->is_active)
    {
      entity_set_target(&entities[i], player->pos);
    }
    else
    {
      entity_set_target(&entities[i], v2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2));
    }

    entity_update(&entities[i], game->t, game->dt);
  }

  // Entity collision detection and resolution
  for (u8 i = 1; i < game->entity_count; i++)
  {
    if (rect_ranges_intersect(
          player->pos, 
          entities[i].pos, 
          player->width,
          player->height,
          entities[i].width, 
          entities[i].height))
    {
      log_float("Delay: ", cur_delay);

      // NOTE: Just no
      if (cur_delay <= 0.0f)
      {
        entity_deal_damage(player);
        cur_delay = max_delay;
      }
      else
      {
        cur_delay -= game->dt;
      }
    }

    for (u8 j = 1; j < game->entity_count; j++)
    {
      if (i == j) continue;
    }
  }

  // Window edge behavior
  if (player->pos.x + player->width <= 0.0f)
  {
    player->pos.x = WINDOW_WIDTH;
  }
  else if (player->pos.x >= WINDOW_WIDTH)
  {
    player->pos.x = 0.0f;
  }
  else if (player->pos.y + player->height <= 0.0f)
  {
    player->pos.y = WINDOW_HEIGHT;
  }
  else if (player->pos.y >= WINDOW_HEIGHT)
  {
    player->pos.y = 0.0f;
  }
}

void game_draw(Game *game)
{
  clear_background(game->renderer, COLOR_PURPLE);

  for (u8 i = 0; i < game->entity_count; i++)
  {
    if (!game->entities[i].is_active) continue;

    draw_rect(
      game->renderer, 
      game->entities[i].pos, 
      game->entities[i].width, 
      game->entities[i].height, 
      game->entities[i].color
    );
  }
}
bool game_should_quit(void)
{
  return input->escape;
}
