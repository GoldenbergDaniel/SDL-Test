#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "sdl2/SDL.h"
#include "glad/glad.h"
#define STB_SPRINTF_IMPLEMENTATION
#include "stb/stb_sprintf.h"
#include "base/base_inc.h"

#include "gfx/render.h"
#include "gfx/draw.h"
#include "input.h"
#include "game.h"
#include "global.h"

// #define PERF

#define TARGET_FPS 60
#define VSYNC 1

Global *GLOBAL;

#ifdef _WIN32
i32 WINAPI WinMain(HINSTANCE inst, HINSTANCE p_inst, LPSTR cmd_line, i32 cmd_show)
#else
i32 main(void)
#endif
{
  Game game = {0};
  game.perm_arena = arena_create(KiB(16));
  game.frame_arena = arena_create(MiB(8));
  game.entity_arena = arena_create(MiB(64));

  char org[128] = {0};
  char app[128] = {0};
  SDL_GetPrefPath(org, app);
  char path[128] = {0};
  stbsp_sprintf(path, "%s/%s\n", org, app);
  printf("Path: %s\n", path);
  FILE *f = fopen(path, "w");
  fprintf(f, "%s\n", path);

  srand(time(NULL));
  arena_get_scratch(NULL);

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
                                        "UNDEAD WEST",
                                        SDL_WINDOWPOS_CENTERED, 
                                        SDL_WINDOWPOS_CENTERED, 
                                        WIDTH, 
                                        HEIGHT, 
                                        SDL_WINDOW_OPENGL);

  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(VSYNC);

  gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);

  String path_to_res = str("res");
  #ifdef DEBUG
  path_to_res = str("../res");
  #endif

  GLOBAL = arena_alloc(&game.frame_arena, sizeof (Global));
  GLOBAL->resources = d_load_resources(&game.frame_arena, path_to_res);
  GLOBAL->renderer = r_create_renderer(40000, &game.frame_arena);

  init_game(&game);

  f64 elapsed_time = 0.0f;
  f64 current_time = SDL_GetTicks64() * 0.001f;
  f64 time_step = 1.0f / TARGET_FPS;
  f64 accumulator = 0.0f;

  game.dt = time_step;

  SDL_PumpEvents();

  bool running = TRUE;
  while (running)
  {
    #ifdef PERF
    u64 perf_start = SDL_GetPerformanceCounter();
    #endif

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
        handle_input_event(&event, &game.should_quit);
      }

      if (game_should_quit(&game))
      {
        running = FALSE;
        break;
      }

      game.t = elapsed_time;

      update_game(&game);
      handle_game_events(&game);

      elapsed_time += time_step;
      accumulator -= time_step;
    }

    draw_game(&game);
    SDL_GL_SwapWindow(window);

    arena_clear(&game.frame_arena);

    #ifdef PERF
    u64 perf_end = SDL_GetPerformanceCounter();
    f64 perf = ((f32) (perf_end - perf_start) / SDL_GetPerformanceFrequency()) * 1000.0f;
    printf("%.0f ms\n", perf);
    // printf("%u fps\n", (u32) (1000 / perf));
    #endif
  }

  SDL_DestroyWindow(window);
  // SDL_Quit();

  return 0;
}
