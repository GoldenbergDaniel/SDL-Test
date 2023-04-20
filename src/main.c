#include <stdio.h>

#include "globals.h"
#include "game.h"

i32 main(i32 argc, i8* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("ERROR: SDL failed to initialize!");

        return 0;
    }

    Game game;

    u32 window_flags = SDL_WINDOW_RESIZABLE;
    u32 renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

    game.window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags);
    game.renderer = SDL_CreateRenderer(game.window, -1, renderer_flags);

    if (game.window == NULL || game.renderer == NULL)
    {
        printf("ERROR: The window or renderer failed to initialize!");

        return 0;
    }

    u64 frame_time;
    u64 last_frame_time = SDL_GetTicks64();
    f32 frame_time_since_update = 0.0f;
    f32 delta_time;

    game.is_running = true;

    game_init(&game);

    int counter = 0;

    while (game.is_running)
    {
        frame_time = SDL_GetTicks64();
        delta_time = frame_time - last_frame_time;
        last_frame_time = frame_time;
        frame_time_since_update += delta_time;

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            game_handle_events(&game, &event);
        }

        while (frame_time_since_update >= (f32) 1 / TARGET_FPS * MILLISECONDS)
        {
            game.time.delta = frame_time_since_update;
            game.time.elapsed = frame_time;
            counter++;
            
            printf("Frame: %i \n", counter);
            printf("Delta: %f \n", game.time.delta);
            
            game_update(&game);

            frame_time_since_update -= (f32) 1 / TARGET_FPS * MILLISECONDS;
        }

        game_draw(&game);

        SDL_SetRenderDrawColor(game.renderer, COLOR_BLUE.r, COLOR_BLUE.g, COLOR_BLUE.b, 255);
        SDL_RenderClear(game.renderer);
    }

    game_uninit(&game);

    SDL_DestroyWindow(game.window);
    SDL_DestroyRenderer(game.renderer);
    SDL_Quit();

    return 0;
}
