#pragma once

typedef struct Game
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool is_running;
    f64 t;
    f64 dt;
    u8 entity_count;
    Entity entities[2];
    Entity *player;
} Game;

void init(Game *game);
void handle_event(Game *game, SDL_Event *event);
void update(Game *game);
void draw(Game *game);
void deinit(void);
