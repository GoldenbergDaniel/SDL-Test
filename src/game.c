#include "SDL2/SDL.h"

#include "globals.h"
#include "draw.h"
#include "util.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "game.h"

extern Input *input;

void init(Game *game)
{
    game->player = create_player(20, 20, COLOR_WHITE);
    game->enemies[0] = create_enemy(20, 20, COLOR_RED);
    
    init_player(&game->player);
    init_enemy(&game->enemies[0]);
}

void handle_event(Game *game, SDL_Event *event)
{   
    switch (event->type)
    {
        case SDL_QUIT: { game->is_running = FALSE; } break;
        case SDL_KEYDOWN:
            switch (event->key.keysym.scancode)
            {
                case SDL_SCANCODE_ESCAPE: { input->escape = TRUE; } break;
                case SDL_SCANCODE_SPACE: { input->space = TRUE; } break;
                case SDL_SCANCODE_A: { input->a = TRUE; } break;
                case SDL_SCANCODE_D: { input->d = TRUE; } break;
                case SDL_SCANCODE_S: { input->s = TRUE; } break;
                case SDL_SCANCODE_W: { input->w = TRUE; } break;
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event->key.keysym.scancode)
            {
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

void update(Game *game)
{
    if (input->escape)
        game->is_running = FALSE;

    update_player(&game->player, game->t, game->dt);
    update_enemy(&game->enemies[0], game->t, game->dt);

    // TODO: Fix clipping bug
    if (game->player.pos.x + game->player.width <= 0.0f)
    {
        game->player.pos.x = WINDOW_WIDTH;
    }
    else if (game->player.pos.x >= WINDOW_WIDTH)
    {
        game->player.pos.x = 0.0f;
    }
    else if (game->player.pos.y + game->player.height <= 0.0f)
    {
        game->player.pos.y = WINDOW_HEIGHT;
    }
    else if (game->player.pos.y >= WINDOW_HEIGHT)
    {
        game->player.pos.y = 0.0f;
    }
}

void draw(Game *game)
{
    clear_background(game->renderer, COLOR_BLACK);

    for (u8 i = 0; i < len(game->enemies); i++)
    {
        Enemy enemy = game->enemies[i];
        draw_rect(game->renderer, enemy.pos, enemy.width, enemy.height, enemy.color);
    }

    draw_rect(game->renderer, game->player.pos, game->player.width, game->player.height, game->player.color);
}
