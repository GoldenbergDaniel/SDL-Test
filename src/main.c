#include <SDL2/SDL.h>

#include "base/base_common.h"

#include "global.h"
#include "game.h"

Global *GLOBAL;

static void set_gl_attributes(void);
static void handle_input(SDL_Event *event, Game *game);

i32 main(void)
{
  Game game = {0};
  game.arena = arena_create(MEGABYTES(16));

  srand(time(NULL));
  SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  set_gl_attributes();

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

  GLOBAL = arena_alloc(&game.arena, sizeof (Global));
  GLOBAL->input = arena_alloc(&game.arena, sizeof (Input));
  GLOBAL->renderer = arena_alloc(&game.arena, sizeof (*GLOBAL->renderer));
  d_init_renderer(GLOBAL->renderer);

  init(&game);

  f64 elapsed_time = 0.0f;
  f64 current_time = SDL_GetTicks64() * 0.001f;
  f64 time_step = 1.0f / TARGET_FPS;
  f64 accumulator = 0.0f;

  while (game.running)
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
        handle_input(&event, &game);
      }

      if (should_quit(&game))
      {
        game.running = FALSE;
      }

      if (!game.first_frame)
      {
        game.t = elapsed_time;
        game.dt = time_step;

        update(&game);
        handle_events(&game);
      }

      elapsed_time += time_step;
      accumulator -= time_step;

      game.first_frame = FALSE;
    }

    draw(&game);
    SDL_GL_SwapWindow(window);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

static
void set_gl_attributes(void)
{
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
}

static
void handle_input(SDL_Event *event, Game *game)
{
  Input *input = GLOBAL->input;

  SDL_GetMouseState(&input->mouse_pos.x, &input->mouse_pos.y);

  switch (event->type)
  {
    case SDL_QUIT: game->should_quit = TRUE;
    break;
    case SDL_KEYDOWN: 
    {
      switch (event->key.keysym.scancode)
      {
        default: break;
        case SDL_SCANCODE_A:
        {
          if (!input->key_down[KEY_A])
            input->key_just_down[KEY_A] = TRUE;

          input->key_down[KEY_A] = TRUE;
        }
        break;
        case SDL_SCANCODE_D:
        {
          if (!input->key_down[KEY_D])
            input->key_just_down[KEY_D] = TRUE;

          input->key_down[KEY_D] = TRUE;
        }
        break;
        case SDL_SCANCODE_S:
        {
          if (!input->key_down[KEY_S])
            input->key_just_down[KEY_S] = TRUE;

          input->key_down[KEY_S] = TRUE;
        }
        break;
        case SDL_SCANCODE_W: 
        {
          if (!input->key_down[KEY_W])
            input->key_just_down[KEY_W] = TRUE;

          input->key_down[KEY_W] = TRUE;
        }
        break;
        case SDL_SCANCODE_ESCAPE:
        {
          if (!input->key_down[KEY_ESCAPE])
            input->key_just_down[KEY_ESCAPE] = TRUE;

          input->key_down[KEY_ESCAPE] = TRUE;
        }
        break;
        case SDL_SCANCODE_SPACE:
        {
          if (!input->key_down[KEY_SPACE])
            input->key_just_down[KEY_SPACE] = TRUE;

          input->key_down[KEY_SPACE] = TRUE;
        }
        break;
        case SDL_SCANCODE_RETURN:
        {
          if (!input->key_down[KEY_ENTER])
            input->key_just_down[KEY_ENTER] = TRUE;

          input->key_down[KEY_ENTER] = TRUE;
        }
        break;
        case SDL_SCANCODE_BACKSPACE:
        {
          if (!input->key_down[KEY_BACKSPACE])
            input->key_just_down[KEY_BACKSPACE] = TRUE;

          input->key_down[KEY_BACKSPACE] = TRUE;
        }
      }
      break;
    }
    case SDL_KEYUP: 
    {
      switch (event->key.keysym.scancode)
      {
        default: break;
        case SDL_SCANCODE_A:
        {
          if (input->key_down[KEY_A])
            input->key_just_up[KEY_A] = TRUE;

          input->key_down[KEY_A] = FALSE;
        }
        break;
        case SDL_SCANCODE_D:
        {
          if (input->key_down[KEY_D])
            input->key_just_up[KEY_D] = TRUE;

          input->key_down[KEY_D] = FALSE;
        }
        break;
        case SDL_SCANCODE_S:
        {
          if (input->key_down[KEY_S])
            input->key_just_up[KEY_S] = TRUE;

          input->key_down[KEY_S] = FALSE;
        }
        break;
        case SDL_SCANCODE_W: 
        {
          if (input->key_down[KEY_W])
            input->key_just_up[KEY_W] = TRUE;

          input->key_down[KEY_W] = FALSE;
        }
        break;
        case SDL_SCANCODE_ESCAPE:
        {
          if (input->key_down[KEY_ESCAPE])
            input->key_just_up[KEY_ESCAPE] = TRUE;

          input->key_down[KEY_ESCAPE] = FALSE;
        }
        break;
        case SDL_SCANCODE_SPACE:
        {
          if (input->key_down[KEY_SPACE])
            input->key_just_up[KEY_SPACE] = TRUE;

          input->key_down[KEY_SPACE] = FALSE;
        }
        break;
        case SDL_SCANCODE_RETURN:
        {
          if (input->key_down[KEY_ENTER])
            input->key_just_up[KEY_ENTER] = TRUE;

          input->key_down[KEY_ENTER] = FALSE;
        }
        break;
        case SDL_SCANCODE_BACKSPACE:
        {
          if (input->key_down[KEY_ENTER])
            input->key_just_up[KEY_ENTER] = TRUE;

          input->key_down[KEY_ENTER] = FALSE;
        }
      }
      break;
    }
  }
}
