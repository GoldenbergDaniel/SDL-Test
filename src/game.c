#include "game.h"

void game_init(Game* game)
{
    
}

void game_handle_events(Game* game, SDL_Event* event)
{
    if (event->type == SDL_QUIT) game->is_running = false;
}

void game_update(Game* game)
{

}

void game_draw(Game* game)
{

}

void game_uninit(Game* game)
{

}
