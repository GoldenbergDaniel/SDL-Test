#include "globals.h"
#include "draw.h"
#include "util.h"
#include "input.h"
#include "player.h"
#include "game.h"

extern Input *input;

void init(Game *game)
{
    game->player = create_player(20, 20, COLOR_RED);
    init_player(&game->player);
}

void handle_event(Game *game, SDL_Event *event)
{   
    switch (event->type)
    {
        case SDL_QUIT:
            game->is_running = FALSE;
            break;
        case SDL_KEYDOWN:
            switch (event->key.keysym.scancode)
            {
                case SDL_SCANCODE_ESCAPE:
                    input->escape = TRUE;
                    break;
                case SDL_SCANCODE_SPACE:
                    input->space = TRUE;
                    break;
                case SDL_SCANCODE_W:
                    input->w = TRUE;
                    break;
                case SDL_SCANCODE_A:
                    input->a = TRUE;
                    break;
                case SDL_SCANCODE_S:
                    input->s = TRUE;
                    break;
                case SDL_SCANCODE_D:
                    input->d = TRUE;
                    break;
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event->key.keysym.scancode)
            {
                case SDL_SCANCODE_ESCAPE:
                    input->escape = FALSE;
                    break;
                case SDL_SCANCODE_SPACE:
                    input->space = FALSE;
                    break;
                case SDL_SCANCODE_W:
                    input->w = FALSE;
                    break;
                case SDL_SCANCODE_A:
                    input->a = FALSE;
                    break;
                case SDL_SCANCODE_S:
                    input->s = FALSE;
                    break;
                case SDL_SCANCODE_D:
                    input->d = FALSE;
                    break;
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
}

void draw(Game *game)
{
    clear_background(game->renderer, COLOR_BLACK);
    draw_player(&game->player, game->renderer);
}
