#pragma once

typedef struct Game Game;
struct Game
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool is_running;
    f64 t;
    f64 dt;
    u8 entity_count;
    u8 enemy_count;
    Entity entities[255];
    Entity *player;
};

void game_init(Game *game);
void game_handle_event(Game *game, SDL_Event *event);
void game_update(Game *game);
void game_draw(Game *game);
bool game_should_quit(void);
