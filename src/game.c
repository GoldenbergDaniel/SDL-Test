#include "globals.h"
#include "util.h"

#include "game.h"

void init(Game *game)
{
    
}

void handle_events(Game *game, SDL_Event *event)
{
    if (event->type == SDL_QUIT) game->is_running = FALSE;
}

void update(Game *game)
{

}

void draw(Game *game)
{

}

void uninit(Game *game)
{

}
