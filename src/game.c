#include "globals.h"
#include "draw.h"
#include "util.h"
#include "input.h"
#include "object.h"
#include "game.h"

#include <stdio.h>

void init(Game *game)
{
    game->object = new_object(20, 20, COLOR_RED);
    init_object(&game->object);
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
                    game->is_running = FALSE;
                    break;
                case SDL_SCANCODE_SPACE:
                    printf("Space key pressed\n");
                    break;
                break;
            }
        break;
    }
}

void update(Game *game)
{
    update_object(&game->object, game->t, game->dt);
}

void draw(Game *game)
{
    clear_background(game->renderer, COLOR_BLACK);

    draw_object(&game->object, game->renderer);
}

void uninit(Game *game)
{

}
