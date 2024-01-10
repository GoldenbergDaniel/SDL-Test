#include "sdl2/SDL.h"
#include "base/base_common.h"
#include "base/base_string.h"
#include "base/base_math.h"

#include "gfx/render.h"
#include "gfx/draw.h"
#include "input.h"
#include "game.h"
#include "global.h"

#define TARGET_FPS 60
#define VSYNC_VAR -1
#define VSYNC_OFF 0
#define VSYNC_ON 1

extern const char *primitive_vert_src;
extern const char *primitive_frag_src;
extern const char *sprite_vert_src;
extern const char *sprite_frag_src;

Global *GLOBAL;

i32 main(void)
{
  Game game = {0};
  game.perm_arena = arena_create(MiB(8));
  game.frame_arena = arena_create(MiB(8));
  game.entity_arena = arena_create(MiB(16));

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
                                        "GAME",
                                        SDL_WINDOWPOS_CENTERED, 
                                        SDL_WINDOWPOS_CENTERED, 
                                        WIDTH, 
                                        HEIGHT, 
                                        SDL_WINDOW_OPENGL);

  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(VSYNC_ON);

  gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);

  GLOBAL = arena_alloc(&game.perm_arena, sizeof (Global));
  GLOBAL->resources = d_load_resources(&game.perm_arena, str("res/"));
  GLOBAL->renderer = r_create_renderer(60000, &game.perm_arena);

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
    f64 new_time = SDL_GetTicks64() * 0.001f;
    f64 frame_time = new_time - current_time;
    current_time = new_time;
    
    accumulator += frame_time;

    while (accumulator >= time_step)
    {
      u64 perf_start = SDL_GetPerformanceCounter();

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
      arena_clear(&game.frame_arena);

      draw_game(&game);
      SDL_GL_SwapWindow(window);

      elapsed_time += time_step;
      accumulator -= time_step;

      u64 perf_end = SDL_GetPerformanceCounter();
      f64 perf = ((f32) (perf_end - perf_start) / SDL_GetPerformanceFrequency()) * 1000.0f;
      printf("%u FPS\n", (u32) (1000/perf));
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
