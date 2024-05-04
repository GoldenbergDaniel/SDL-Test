#include <stdlib.h>
#include <time.h>

#include "SDL3/SDL_init.h"
#include "SDL3/SDL_video.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_timer.h"
#include "glad/glad.h"

#include "base/base_inc.h"
#include "render/render.h"
#include "draw/draw.h"
#include "input/input.h"
#include "game.h"
#include "global.h"

// #define LOG_PERF

#define SIM_RATE 60
#define VSYNC 1

Global *GLOBAL;

i32 main(void)
{
  Game game = {0};
  game.perm_arena = arena_create(MiB(16));
  game.frame_arena = arena_create(MiB(16));
  game.batch_arena = arena_create(MiB(16));
  game.entity_arena = arena_create(MiB(64));

  srand(time(NULL));
  arena_get_scratch(NULL);

  SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

  i32 gl_major_version = 4;
  #ifdef _WIN32
  i32 gl_minor_version = 6;
  #else
  i32 gl_minor_version = 1;
  #endif

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_version);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_version);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

  #ifdef _WIN32
  SDL_WindowFlags flags = SDL_WINDOW_OPENGL;
  #else
  SDL_WindowFlags flags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY;
  #endif

  SDL_Window *window = SDL_CreateWindow("Undead West", WIDTH, HEIGHT, flags);

  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(VSYNC);

  gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);
  d_clear_frame(V4F_ZERO);

  String path_to_res = str("res");
  #ifdef DEBUG
  path_to_res = str("../res");
  #endif

  GLOBAL = arena_alloc(&game.perm_arena, sizeof (Global));
  GLOBAL->resources = d_load_resources(&game.perm_arena, path_to_res);
  GLOBAL->renderer = r_create_renderer(40000, &game.batch_arena);

  Game prev_game = {0};
  prev_game.perm_arena = arena_create(KiB(16));
  prev_game.frame_arena = arena_create(MiB(8));
  prev_game.batch_arena = arena_create(MiB(8));
  prev_game.entity_arena = arena_create(MiB(64));

  init_game(&game);

  f64 elapsed_time = 0.0f;
  f64 current_time = SDL_GetTicks() * 0.001f;
  f64 time_step = 1.0f / SIM_RATE;
  f64 accumulator = 0.0f;

  game.dt = time_step;

  bool running = TRUE;
  while (running)
  {
    #ifdef LOG_PERF
    u64 perf_start = SDL_GetPerformanceCounter();
    #endif

    f64 new_time = SDL_GetTicks() * 0.001f;
    f64 frame_time = new_time - current_time;
    current_time = new_time;
    accumulator += frame_time;

    while (accumulator >= time_step)
    {
      clear_last_frame_input();

      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        handle_input_event(&event, &game.should_quit);
      }

      if (game_should_quit(&game))
      {
        running = FALSE;
        break;
      }

      game.t = elapsed_time;

      // copy_game_state(&game, &prev_game);
      update_game(&game);
      handle_game_events(&game);
      arena_clear(&game.frame_arena);

      elapsed_time += time_step;
      accumulator -= time_step;
    }

    draw_game(&game, &prev_game);
    SDL_GL_SwapWindow(window);
    arena_clear(&game.batch_arena);

    // clear prev game arenas
    arena_clear(&prev_game.frame_arena);
    arena_clear(&prev_game.batch_arena);
    arena_clear(&prev_game.entity_arena);

    #ifdef LOG_PERF
    u64 perf_end = SDL_GetPerformanceCounter();
    f64 perf = ((f32) (perf_end - perf_start) / SDL_GetPerformanceFrequency()) * 1000.0f;
    printf("%.0f ms\n", perf);
    // printf("%u fps\n", (u32) (1000 / perf));
    #endif
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
