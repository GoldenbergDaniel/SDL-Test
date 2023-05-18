#include <stdio.h>

#include "globals.h"
#include "util.h"
#include "game.h"

i32 main()
{
    Game game;
    game.window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS);
    game.renderer = SDL_CreateRenderer(game.window, -1, RENDERER_FLAGS);
    game.is_running = TRUE;

    init(&game);

    f64 elapsed_time = 0.0f;
    f64 current_time = time_in_seconds();
    f64 time_step = 1.0f / TARGET_FPS;
    f64 accumulator = 0.0f;

    while (game.is_running)
    {
        f64 new_time = time_in_seconds();
        f64 frame_time = new_time - current_time;

        current_time = new_time;
        accumulator += frame_time;

        while (accumulator >= time_step)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event)) handle_event(&game, &event);

            game.t = elapsed_time;
            game.dt = time_step;

            if (elapsed_time >= 1.0f)
                update(&game);

            elapsed_time += time_step;
            accumulator -= time_step;
        }
        
        draw(&game);
        SDL_RenderPresent(game.renderer);
    }

    uninit(&game);
    SDL_DestroyWindow(game.window);
    SDL_DestroyRenderer(game.renderer);
    SDL_Quit();

    return 0;
}
