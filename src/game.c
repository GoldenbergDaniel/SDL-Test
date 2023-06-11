#include <SDL2/SDL.h>

#include "globals.h"
#include "util.h"
#include "draw.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "game.h"

Input *input;

void init(Game *game)
{   
    input = (Input*) malloc(sizeof(*input));

    game->player = create_player(20, 20, COLOR_WHITE);

    for (u8 i = 0; i < array_len(game->enemies); i++)
    {
        game->enemies[i] = create_enemy(40, 40, COLOR_RED);
    }

    // game->enemies[1].color = COLOR_BLUE;

    init_player(&game->player);

    for (u8 i = 0; i < array_len(game->enemies); i++)
    {
        init_enemy(&game->enemies[i]);
    }
}

void handle_event(Game *game, SDL_Event *event)
{   
    switch (event->type)
    {
        case SDL_QUIT: { game->is_running = FALSE; } break;
        case SDL_KEYDOWN: 
        {
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
        }
        case SDL_KEYUP: 
        {
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
}

void update(Game *game)
{
    if (input->escape)
    {
        game->is_running = FALSE;
    }

    update_player(&game->player, game->t, game->dt);

    for (u8 i = 0; i < array_len(game->enemies); i++)
    {
        v2f p_center = get_rect_center(
                            game->player.pos, 
                            game->player.width, 
                            game->player.height
                       );

        enemy_find_target(&game->enemies[i], &p_center);

        update_enemy(&game->enemies[i], game->t, game->dt);

        // Player-Enemy collision
        if (rr_collision(
                add_v2f(game->enemies[i].pos, game->enemies[i].vel),
                add_v2f(game->player.pos, game->player.vel),
                game->enemies[i].width,
                game->enemies[i].height,
                game->player.width,
                game->player.height
            ))
        {   
            // NOTE: These work exclusive or (XOR)
            // Idea: Do something with the vectors from center of one rect to other

            // if (abs(game->player.pos.x + game->player.width - game->enemies[0].pos.x) >
            //     abs(game->player.pos.y + game->player.height - game->enemies[0].pos.y)
            //    )
            // {
            //     game->player.pos.x = game->enemies[0].pos.x + game->enemies[0].width;
            // }
            // else
            // {
            //     game->player.pos.y = game->enemies[0].pos.y + game->enemies[0].height;
            // }

            if (abs(game->enemies[0].pos.x + game->enemies[0].width - game->player.pos.x) >
                abs(game->enemies[0].pos.y + game->enemies[0].height - game->player.pos.y)
               )
            {
                game->player.pos.x = game->enemies[0].pos.x - game->player.width;
            }
            else
            {
                game->player.pos.y = game->enemies[0].pos.y - game->player.height;
            }
        }

        for (u8 j = 0; j < array_len(game->enemies); j++)
        {
            if (i == j) continue;

            // Enemy-Enemy collision
            if (rr_collision(
                    add_v2f(game->enemies[i].pos, game->enemies[i].vel),
                    add_v2f(game->enemies[j].pos, game->enemies[j].vel),
                    game->enemies[i].width,
                    game->enemies[i].height,
                    game->enemies[j].width,
                    game->enemies[j].height
                ))
            {
                // game->enemies[i].color = COLOR_BLUE;
            }
        }
    }

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

    // Draw player
    draw_rect(
        game->renderer, 
        game->player.pos, 
        game->player.width,
        game->player.height, 
        game->player.color
    );

    // Draw enemies
    for (u8 i = 0; i < array_len(game->enemies); i++)
    {
        draw_rect(
            game->renderer, 
            game->enemies[i].pos, 
            game->enemies[i].width, 
            game->enemies[i].height, 
            game->enemies[i].color
        );
    }
}
