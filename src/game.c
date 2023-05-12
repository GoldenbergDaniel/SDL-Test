#include "globals.h"
#include "util.h"
#include "draw.h"

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
    clear_background(game->renderer, COLOR_BLACK);

    draw_rect(game->renderer, (v2) {WINDOW_WIDTH/2 - 10, WINDOW_HEIGHT/2 - 10}, 20, 20, COLOR_RED);
}

void uninit(Game *game)
{

}
