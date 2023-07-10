#include <SDL2/SDL.h>

#include "globals.h"
#include "util.h"
#include "draw.h"
#include "input.h"
#include "entity.h"
#include "game.h"

Input *input;

void game_init(Game *game)
{   
    input = (Input *) malloc(sizeof (Input *));

    // Create entities
    game->enemy_count = 10;
    game->entity_count = game->enemy_count+1;
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
    // Update entities
    entity_update(game->player, game->t, game->dt);
    
    for (u8 i = 1; i < game->entity_count; i++)
    {
        entity_set_target(&game->entities[i], game->player->pos);
        entity_update(&game->entities[i], game->t, game->dt);
    }

    // Collision detection and resoluton
    for (u8 i = 0; i < game->entity_count; i++)
    {
        for (u8 j = 0; j < game->entity_count; j++)
        {
            if (i == j) continue;
        }
    }

    // Window edge behavior
    if (game->player->pos.x + game->player->width <= 0.0f)
    {
        game->player->pos.x = WINDOW_WIDTH;
    }
    else if (game->player->pos.x >= WINDOW_WIDTH)
    {
        game->player->pos.x = 0.0f;
    }
    else if (game->player->pos.y + game->player->height <= 0.0f)
    {
        game->player->pos.y = WINDOW_HEIGHT;
    }
    else if (game->player->pos.y >= WINDOW_HEIGHT)
    {
        game->player->pos.y = 0.0f;
    }
}

void game_draw(Game *game)
{
    clear_background(game->renderer, COLOR_PURPLE);

    for (u8 i = 0; i < game->entity_count; i++)
    {
        draw_rect(
            game->renderer, 
            game->entities[i].pos, 
            game->entities[i].width, 
            game->entities[i].height, 
            game->entities[i].color
        );
    }
}

void game_deinit(void)
{
    free(input);
}

bool game_should_quit(void)
{
    return input->escape;
}
