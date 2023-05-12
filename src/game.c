#include "globals.h"
#include "draw.h"
#include "util.h"
#include "object.h"
#include "game.h"

void init(Game *game)
{
    game->object = new_object(20, 20, COLOR_RED);
    init_object(&game->object);
}

void handle_events(Game *game, SDL_Event *event)
{
    if (event->type == SDL_QUIT) 
        game->is_running = FALSE;
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
