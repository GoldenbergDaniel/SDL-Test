#include <stdio.h>

#include "globals.h"
#include "game.h"

i32 main(i32 argc, i8* argv[])
{
    Game game;
    game.window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS);
    game.renderer = SDL_CreateRenderer(game.window, -1, RENDERER_FLAGS);
    game.is_running = true;

    game_init(&game);

    f64 t = 0.0f;
    f64 dt = 1.0 / 60.0f;

    while (game.is_running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) game_handle_events(&game, &event);

        game.time.elapsed = t;
        game.time.delta = dt;
        
        game_update(&game);
        game_draw(&game);

        SDL_SetRenderDrawColor(game.renderer, COLOR_BLUE.r, COLOR_BLUE.g, COLOR_BLUE.b, 255);
        SDL_RenderClear(game.renderer);
        SDL_RenderPresent(game.renderer);

        t += dt;
    }

    game_uninit(&game);

    SDL_DestroyWindow(game.window);
    SDL_DestroyRenderer(game.renderer);
    SDL_Quit();

    return 0;
}
