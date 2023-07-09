#include <SDL2/SDL.h>

#include "globals.h"
#include "util.h"
#include "draw.h"
#include "input.h"
#include "entity.h"
#include "game.h"

Input *input;

void init(Game *game)
{   
    input = (Input*) malloc(sizeof (Input*));

    game->entity_count = arr_len(game->entities);
    game->entities[0] = entity_create(ENTITY_PLAYER);
    entity_init(&game->entities[0]);

    for (u8 i = 1; i < game->entity_count; i++)
    {
        game->entities[i] = entity_create(ENTITY_ENEMY);
        entity_init(&game->entities[i]);
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

void update(Game *game)
{
    if (input->escape)
        game->is_running = FALSE;

    game->player = &game->entities[0];
    entity_update(game->player, game->t, game->dt);

    for (u8 i = 1; i < game->entity_count; i++)
    {
        entity_set_target(&game->entities[i], game->player->pos);
        entity_update(&game->entities[i], game->t, game->dt);
    }

    // for (u8 i = 0; i < array_len(game->enemies); i++)
    // {
    //     v2f p_center = get_rect_center(
    //             game->player.pos, 
    //             game->player.width, 
    //             game->player.height
    //         );

        // Entity-Entity collision
        // if (rr_collision(
        //         add_v2f(game->enemies[i].pos, game->enemies[i].vel),
        //         add_v2f(game->player.pos, game->player.vel),
        //         game->enemies[i].width,
        //         game->enemies[i].height,
        //         game->player.width,
        //         game->player.height
        //     ))
        // {
            // if (abs((p->pos.x + p->width - e[0].pos.x)) > 0 && 
            //     (abs(p->pos.x + p->width - e[0].pos.x)) < p->width)
            // {
            //     p->pos.x = e[0].pos.x - p->width;
            // }

            // if (abs(((e[0].pos.x + e[0].width) - p->pos.x)) > 0 && 
            //     (abs((e[0].pos.x + e[0].width) - p->pos.x)) < p->width) 
            // {
            //     p->pos.x = e[0].pos.x + e[0].width;
            // }

            // if (abs((p->pos.y + p->height) - e[0].pos.y) > 0 && 
            //     (abs((p->pos.y + p->height) - e[0].pos.y)) < p->height) 
            // {
            //     p->pos.y = e[0].pos.y - p->height;
            // }

            // if (abs((e[0].pos.y + e[0].height) - p->pos.y) > 0 && 
            //     (abs((e[0].pos.y + e[0].height) - p->pos.y)) < p->height) 
            // {
            //     p->pos.y = e[0].pos.y + e[0].height;
            // }
        // }
    // }

    // TODO: Fix clipping bug
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

void draw(Game *game)
{
    clear_background(game->renderer, COLOR_BLACK);

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

void deinit(void)
{
    free(input);
}
