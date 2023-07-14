#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "globals.h"
#include "util.h"
#include "component.h"
#include "entity.h"
#include "game.h"
#include "main.h"

i32 main(void)
{
  Game game;
  game.window = SDL_CreateWindow(
                  WINDOW_NAME, 
                  SDL_WINDOWPOS_CENTERED, 
                  SDL_WINDOWPOS_CENTERED, 
                  WINDOW_WIDTH, 
                  WINDOW_HEIGHT, 
                  WINDOW_FLAGS
  );

  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
  game.renderer = SDL_CreateRenderer(game.window, -1, RENDERER_FLAGS);
  game.is_running = TRUE;

  srand(time(NULL));

  game_init(&game);
  
  f64 elapsed_time = 0.0f;
  f64 current_time = SDL_GetTicks64() * 0.001f;
  f64 time_step = 1.0f / TARGET_FPS;
  f64 accumulator = 0.0f;

  while (game.is_running)
  {
    f64 new_time = SDL_GetTicks64() * 0.001f;
    f64 frame_time = new_time - current_time;

    current_time = new_time;
    accumulator += frame_time;

    while (accumulator >= time_step)
    {
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        game_handle_event(&game, &event);
      }

      if (game_should_quit())
      {
        game.is_running = FALSE;
      }

      game.t = elapsed_time;
      game.dt = time_step;
      game_update(&game);

      elapsed_time += time_step;
      accumulator -= time_step;
    }

    game_draw(&game);
    SDL_RenderPresent(game.renderer);
  }

  SDL_DestroyWindow(game.window);
  SDL_DestroyRenderer(game.renderer);
  SDL_Quit();

  return 0;
}
