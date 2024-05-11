#include "SDL3/SDL_events.h"
#include "../base/base_inc.h"

#include "../global.h"
#include "input.h"

extern Global *GLOBAL;

inline
bool is_key_pressed(InputKey key)
{
  return GLOBAL->input.key_down[key];
}

inline
bool is_key_just_pressed(InputKey key)
{
  return GLOBAL->input.key_just_down[key];
}

inline
bool is_key_just_released(InputKey key)
{
  return GLOBAL->input.key_just_up[key];
}

inline
Vec2F get_mouse_pos(void)
{
  return v2f(GLOBAL->input.mouse_pos.x, GLOBAL->input.mouse_pos.y);
}

inline
void clear_last_frame_input(void)
{
  for (u8 i = 0; i < _KEY_COUNT; i++)
  {
    GLOBAL->input.key_just_down[i] = FALSE;
    GLOBAL->input.key_just_up[i] = FALSE;
  }
}

void handle_input_event(SDL_Event *event, bool *should_quit)
{
  Input *input = &GLOBAL->input;
  SDL_GetMouseState(&input->mouse_pos.x, &input->mouse_pos.y);

  switch (event->type)
  {
    case SDL_EVENT_QUIT: *should_quit = TRUE;
    break;
    case SDL_EVENT_KEY_DOWN: 
    {
      switch (event->key.keysym.scancode)
      {
        default: break;
        case SDL_SCANCODE_A:
        {
          if (!input->key_down[KEY_A]) input->key_just_down[KEY_A] = TRUE;
          input->key_down[KEY_A] = TRUE;
        }
        break;
        case SDL_SCANCODE_D:
        {
          if (!input->key_down[KEY_D]) input->key_just_down[KEY_D] = TRUE;
          input->key_down[KEY_D] = TRUE;
        }
        break;
        case SDL_SCANCODE_S:
        {
          if (!input->key_down[KEY_S]) input->key_just_down[KEY_S] = TRUE;
          input->key_down[KEY_S] = TRUE;
        }
        break;
        case SDL_SCANCODE_W: 
        {
          if (!input->key_down[KEY_W]) input->key_just_down[KEY_W] = TRUE;
          input->key_down[KEY_W] = TRUE;
        }
        break;
        case SDL_SCANCODE_0:
        {
          if (!input->key_down[KEY_0]) input->key_just_down[KEY_0] = TRUE;
          input->key_down[KEY_0] = TRUE;
        }
        break;
        case SDL_SCANCODE_1: 
        {
          if (!input->key_down[KEY_1]) input->key_just_down[KEY_1] = TRUE;
          input->key_down[KEY_1] = TRUE;
        }
        break;
        case SDL_SCANCODE_2: 
        {
          if (!input->key_down[KEY_2]) input->key_just_down[KEY_2] = TRUE;
          input->key_down[KEY_2] = TRUE;
        }
        break;
        case SDL_SCANCODE_3:
        {
          if (!input->key_down[KEY_3]) input->key_just_down[KEY_3] = TRUE;
          input->key_down[KEY_3] = TRUE;
        }
        break;
        case SDL_SCANCODE_9:
        {
          if (!input->key_down[KEY_9]) input->key_just_down[KEY_9] = TRUE;
          input->key_down[KEY_9] = TRUE;
        }
        break;
        case SDL_SCANCODE_ESCAPE:
        {
          if (!input->key_down[KEY_ESCAPE]) input->key_just_down[KEY_ESCAPE] = TRUE;
          input->key_down[KEY_ESCAPE] = TRUE;
        }
        break;
        case SDL_SCANCODE_SPACE:
        {
          if (!input->key_down[KEY_SPACE]) input->key_just_down[KEY_SPACE] = TRUE;
          input->key_down[KEY_SPACE] = TRUE;
        }
        break;
        case SDL_SCANCODE_RETURN:
        {
          if (!input->key_down[KEY_ENTER]) input->key_just_down[KEY_ENTER] = TRUE;
          input->key_down[KEY_ENTER] = TRUE;
        }
        break;
        case SDL_SCANCODE_BACKSPACE:
        {
          if (!input->key_down[KEY_BACKSPACE]) input->key_just_down[KEY_BACKSPACE] = TRUE;
          input->key_down[KEY_BACKSPACE] = TRUE;
        }
        case SDL_SCANCODE_TAB:
        {
          if (!input->key_down[KEY_TAB]) input->key_just_down[KEY_TAB] = TRUE;
          input->key_down[KEY_TAB] = TRUE;
        }
      }
      break;
    }
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    {
      switch (event->button.button)
      {
        default: break;
        case SDL_BUTTON_LEFT:
        {
          if (!input->key_down[KEY_MOUSE_1]) input->key_just_down[KEY_MOUSE_1] = TRUE;
          input->key_down[KEY_MOUSE_1] = TRUE;
        }
        break;
        case SDL_BUTTON_RIGHT:
        {
          if (!input->key_down[KEY_MOUSE_2]) input->key_just_down[KEY_MOUSE_2] = TRUE;
          input->key_down[KEY_MOUSE_2] = TRUE;
        }
        break;
      }
    }
    break;
    case SDL_EVENT_KEY_UP: 
    {
      switch (event->key.keysym.scancode)
      {
        default: break;
        case SDL_SCANCODE_A:
        {
          if (input->key_down[KEY_A]) input->key_just_up[KEY_A] = TRUE;
          input->key_down[KEY_A] = FALSE;
        }
        break;
        case SDL_SCANCODE_D:
        {
          if (input->key_down[KEY_D]) input->key_just_up[KEY_D] = TRUE;
          input->key_down[KEY_D] = FALSE;
        }
        break;
        case SDL_SCANCODE_S:
        {
          if (input->key_down[KEY_S]) input->key_just_up[KEY_S] = TRUE;
          input->key_down[KEY_S] = FALSE;
        }
        break;
        case SDL_SCANCODE_W: 
        {
          if (input->key_down[KEY_W]) input->key_just_up[KEY_W] = TRUE;
          input->key_down[KEY_W] = FALSE;
        }
        break;
        case SDL_SCANCODE_0:
        {
          if (!input->key_down[KEY_0]) input->key_just_up[KEY_0] = TRUE;
          input->key_down[KEY_0] = FALSE;
        }
        break;
        case SDL_SCANCODE_1:
        {
          if (!input->key_down[KEY_1]) input->key_just_up[KEY_1] = TRUE;
          input->key_down[KEY_1] = FALSE;
        }
        break;
        case SDL_SCANCODE_2:
        {
          if (!input->key_down[KEY_2]) input->key_just_up[KEY_2] = TRUE;
          input->key_down[KEY_2] = FALSE;
        }
        break;
        case SDL_SCANCODE_3:
        {
          if (!input->key_down[KEY_3]) input->key_just_up[KEY_3] = TRUE;
          input->key_down[KEY_3] = FALSE;
        }
        break;
        case SDL_SCANCODE_9:
        {
          if (!input->key_down[KEY_9]) input->key_just_up[KEY_9] = TRUE;
          input->key_down[KEY_9] = FALSE;
        }
        break;
        case SDL_SCANCODE_ESCAPE:
        {
          if (input->key_down[KEY_ESCAPE]) input->key_just_up[KEY_ESCAPE] = TRUE;
          input->key_down[KEY_ESCAPE] = FALSE;
        }
        break;
        case SDL_SCANCODE_SPACE:
        {
          if (input->key_down[KEY_SPACE]) input->key_just_up[KEY_SPACE] = TRUE;
          input->key_down[KEY_SPACE] = FALSE;
        }
        break;
        case SDL_SCANCODE_RETURN:
        {
          if (input->key_down[KEY_ENTER]) input->key_just_up[KEY_ENTER] = TRUE;
          input->key_down[KEY_ENTER] = FALSE;
        }
        break;
        case SDL_SCANCODE_BACKSPACE:
        {
          if (input->key_down[KEY_ENTER]) input->key_just_up[KEY_ENTER] = TRUE;
          input->key_down[KEY_ENTER] = FALSE;
        }
        break;
        case SDL_SCANCODE_TAB:
        {
          if (input->key_down[KEY_TAB]) input->key_just_up[KEY_TAB] = TRUE;
          input->key_down[KEY_TAB] = FALSE;
        }
      }
      break;
    }
    break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
    {
      switch (event->button.button)
      {
        default: break;
        case SDL_BUTTON_LEFT:
        {
          if (input->key_down[KEY_MOUSE_1]) input->key_just_up[KEY_MOUSE_1] = TRUE;
          input->key_down[KEY_MOUSE_1] = FALSE;
        }
        break;
        case SDL_BUTTON_RIGHT:
        {
          if (input->key_down[KEY_MOUSE_2]) input->key_just_up[KEY_MOUSE_2] = TRUE;
          input->key_down[KEY_MOUSE_2] = FALSE;
        }
        break;
      }
    }
  }
}
