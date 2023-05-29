#include "SDL2/SDL.h"

#include "globals.h"
#include "draw.h"
#include "util.h"
#include "input.h"
#include "player.h"
#include "game.h"

extern Input *input;

void init(Game *game)
{
    game->player = create_player(20, 20, COLOR_WHITE);
    init_player(&game->player);
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

    // TODO: Fix clipping bug
    if (get_player_right_bound(game->player) <= 0.0f)
    {
        game->player.pos.x = WINDOW_WIDTH;
    }
    else if (game->player.pos.x >= WINDOW_WIDTH)
    {
        game->player.pos.x = 0.0f;
    }
    else if (get_player_bottom_bound(game->player) <= 0.0f)
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
    draw_player(&game->player, game->renderer);
}

f64 time_in_seconds()
{
    return (f64) SDL_GetTicks64() * 0.001f;
}
