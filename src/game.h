#pragma once

#ifndef OBJECT_H
#include "object.h"
#endif

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool is_running;
    f64 t;
    f64 dt;
    Object object;
} Game;

void init(Game *game);
void handle_event(Game *game, SDL_Event *event);
void update(Game *game);
void draw(Game *game);
