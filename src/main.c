#include <SDL2/SDL.h>

#include "base/base_common.h"
#include "base/base_string.h"
#include "base/base_math.h"

#include "input.h"
#include "game.h"

Global *GLOBAL;

i32 main(void)
{
  Game game = {0};
  game.perm_arena = create_arena(MiB(8));
  game.frame_arena = create_arena(MiB(8));
  game.entity_arena = create_arena(MiB(16));

  srand(time(NULL));

  SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

  SDL_Window *window = SDL_CreateWindow(
                                        "SPACE GAME",
                                        SDL_WINDOWPOS_CENTERED, 
                                        SDL_WINDOWPOS_CENTERED, 
                                        W_WIDTH, 
                                        W_HEIGHT, 
                                        SDL_WINDOW_OPENGL);

  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(VSYNC_ON);

  if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress))
  {
    printf("GLAD failed to load!\n");
    return 1;
  }

  GLOBAL = arena_alloc(&game.perm_arena, sizeof (Global));
  GLOBAL->assets = d_load_assets(&game.perm_arena, str("res/"));
  GLOBAL->renderer = d_create_renderer();

  init_game(&game);

  f64 elapsed_time = 0.0f;
  f64 current_time = SDL_GetTicks64() * 0.001f;
  f64 time_step = 1.0f / TARGET_FPS;
  f64 accumulator = 0.0f;

  bool running = TRUE;
  while (running)
  {
    f64 new_time = SDL_GetTicks64() * 0.001f;
    f64 frame_time = new_time - current_time;
    current_time = new_time;
    accumulator += frame_time;

    while (accumulator >= time_step)
    {
      clear_last_frame_input();

      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        handle_input(&event, &game.should_quit);
      }

      if (game_should_quit(&game))
      {
        running = FALSE;
        break;
      }

    if (elapsed_time > 0.1f)
      {
        game.t = elapsed_time;
        game.dt = time_step;

        update_game(&game);
        handle_game_events(&game);
        draw_game(&game);
        clear_game_frame_arena(&game);
        
        SDL_GL_SwapWindow(window);
      }

      elapsed_time += time_step;
      accumulator -= time_step;
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
