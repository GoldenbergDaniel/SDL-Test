#include <SDL2/SDL.h>
#include "base/base_common.h"

// #include "global.h"
#include "game.h"
#include "input.h"

extern Global *GLOBAL;

inline
bool key_pressed(Key key)
{
  return GLOBAL->input->key_down[key];
}

inline
bool key_just_pressed(Key key)
{
  return GLOBAL->input->key_just_down[key];
}

inline
bool key_just_released(Key key)
{
  return GLOBAL->input->key_just_up[key];
}

inline
Vec2I get_mouse_position(void)
{
  return GLOBAL->input->mouse_pos;
}

inline
void clear_last_frame_input(void)
{
  for (u8 i = 0; i < sizeof (GLOBAL->input->key_just_down); i++)
  {
    GLOBAL->input->key_just_down[i] = FALSE;
    GLOBAL->input->key_just_up[i] = FALSE;
  }
}

void handle_input(SDL_Event *event, bool *should_quit)
{
  Input *input = GLOBAL->input;

  SDL_GetMouseState(&input->mouse_pos.x, &input->mouse_pos.y);

  switch (event->type)
  {
    case SDL_QUIT: *should_quit = TRUE;
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
