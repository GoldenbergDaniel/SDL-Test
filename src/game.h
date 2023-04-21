#pragma once

#include "globals.h"
#include "game_time.h"

typedef struct Game
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    GameTime time;
    bool is_running;
} Game;

void game_init(Game* game);
void game_handle_events(Game* game, SDL_Event* event);
void game_update(Game* game);
void game_draw(Game* game);
void game_uninit(Game* game);
