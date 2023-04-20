#include "globals.h"

typedef struct Time
{
    f32 delta;
    u64 elapsed;
} Time;

typedef struct Game
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    Time time;
    bool is_running;
} Game;

void game_init(Game* game);
void game_handle_events(Game* game, SDL_Event* event);
void game_update(Game* game);
void game_draw(Game* game);
void game_uninit(Game* game);
