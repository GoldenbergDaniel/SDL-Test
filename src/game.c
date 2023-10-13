#include <stdio.h>
#include <SDL2/SDL.h>

#include "base_common.h"
#include "base_os.h"
#include "draw.h"
#include "shaders.h"
#include "util.h"
#include "component.h"
#include "entity.h"
#include "game.h"

Input *input;

void game_init(Game *game)
{
  input = (Input *) os_alloc(sizeof (Input *));

  R_Shader shader = r_create_shader(shaders_vert_src, shaders_frag_src);

  R_Vertex vertices[4] = 
  {
    {{-5.0f,  5.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}, // top left
    {{ 5.0f,  5.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}, // top right
    {{ 5.0f, -5.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}, // bottom right
    {{-5.0f, -5.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}  // bottom left
  };

  u16 indices[6] = 
  {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
  };

  R_Object vao = r_create_vertex_array(2);
  r_create_vertex_buffer(vertices, sizeof (vertices));
  r_create_index_buffer(indices, sizeof (indices));
  r_create_vertex_layout(&vao, GL_FLOAT, 3);
  r_create_vertex_layout(&vao, GL_FLOAT, 3);

  game->camera = translate_3x3f(W_WIDTH / 2.0f, W_HEIGHT / 2.0f);
  game->draw_stream = (D_Stream) 
  {
    .vao = vao,
    .shader = shader,
    .camera = game->camera
  };

  // Create entities
  game->enemy_count = 0;
  game->entity_count = game->enemy_count + 1;
  game->entities[0] = entity_create(EntityType_Player);
  game->player = &game->entities[0];

  for (u8 i = 1; i < game->entity_count; i++)
  {
    game->entities[i] = entity_create(EntityType_Enemy);
  }

  // Set starting positions
  for (u8 i = 0; i < game->entity_count; i++)
  {
    entity_start(&game->entities[i]);
  }

  game->running = TRUE;
}

void game_handle_event(Game *game, SDL_Event *event)
{   
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

void game_update(Game *game)
{
  Entity *player = game->player;
  Entity *entities = game->entities;

  // Update entities
  entity_update(player, game->dt);
  
  for (u8 i = 1; i < game->entity_count; i++)
  {
    if (player->is_active)
    {
      entity_set_target(&entities[i], player->pos);
    }
    else
    {
      entity_set_target(&entities[i], v2f(W_WIDTH/2.0f, W_HEIGHT/2.0f));
    }

    entity_update(&entities[i], game->dt);
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
      // NOTE: Broken!
      if (!player->hurt_cooldown.is_running)
      {
        timer_start(&player->hurt_cooldown, FALSE);
      }
      else
      {
        timer_tick(&player->hurt_cooldown, game->dt);

        if (player->hurt_cooldown.timeout)
        {
          print("Timeout!");
        }

        printf("Timer: %f", player->hurt_cooldown.cur_duration);
      }
    }
  }

  // Window edge behavior
  if (player->pos.x + player->width <= 0.0f)
  {
    player->pos.x = W_WIDTH;
  }
  else if (player->pos.x >= W_WIDTH)
  {
    player->pos.x = 0.0f;
  }
  else if (player->pos.y + player->height <= 0.0f)
  {
    player->pos.y = W_HEIGHT;
  }
  else if (player->pos.y >= W_HEIGHT)
  {
    player->pos.y = 0.0f;
  }
}

void game_draw(Game *game)
{
  d_clear(v4f(0.1f, 0.1f, 0.1f, 1.0f));

  for (u8 i = 0; i < game->entity_count; i++)
  {
    Entity entity = game->entities[i];

    if (entity.is_active)
    {
      d_draw_rect(&game->draw_stream, entity.xform, entity.color);
    }
  }
}

bool game_should_quit(void)
{
  return input->escape;
}
