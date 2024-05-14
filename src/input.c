#include "base/base_inc.h"

#include "global.h"
#include "input.h"

#define SOKOL_DLL
#define SOKOL_NO_ENTRY
#include "sokol/sokol_app.h"

#include "sokol/sokol_time.h"

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

void handle_input_event(const struct sapp_event *event, bool *should_quit)
{
  Input *input = &GLOBAL->input;
  input->mouse_pos = v2f(event->mouse_x, event->mouse_y);

  switch (event->type)
  {
    case SAPP_EVENTTYPE_QUIT_REQUESTED: *should_quit = TRUE;
    break;
    case SAPP_EVENTTYPE_KEY_DOWN: 
    {
      switch (event->key_code)
      {
        default: break;
        case SAPP_KEYCODE_A:
        {
          if (!input->key_down[KEY_A]) input->key_just_down[KEY_A] = TRUE;
          input->key_down[KEY_A] = TRUE;
        }
        break;
        case SAPP_KEYCODE_D:
        {
          if (!input->key_down[KEY_D]) input->key_just_down[KEY_D] = TRUE;
          input->key_down[KEY_D] = TRUE;
        }
        break;
        case SAPP_KEYCODE_S:
        {
          if (!input->key_down[KEY_S]) input->key_just_down[KEY_S] = TRUE;
          input->key_down[KEY_S] = TRUE;
        }
        break;
        case SAPP_KEYCODE_W: 
        {
          if (!input->key_down[KEY_W]) input->key_just_down[KEY_W] = TRUE;
          input->key_down[KEY_W] = TRUE;
        }
        break;
        case SAPP_KEYCODE_0:
        {
          if (!input->key_down[KEY_0]) input->key_just_down[KEY_0] = TRUE;
          input->key_down[KEY_0] = TRUE;
        }
        break;
        case SAPP_KEYCODE_1: 
        {
          if (!input->key_down[KEY_1]) input->key_just_down[KEY_1] = TRUE;
          input->key_down[KEY_1] = TRUE;
        }
        break;
        case SAPP_KEYCODE_2: 
        {
          if (!input->key_down[KEY_2]) input->key_just_down[KEY_2] = TRUE;
          input->key_down[KEY_2] = TRUE;
        }
        break;
        case SAPP_KEYCODE_3:
        {
          if (!input->key_down[KEY_3]) input->key_just_down[KEY_3] = TRUE;
          input->key_down[KEY_3] = TRUE;
        }
        break;
        case SAPP_KEYCODE_9:
        {
          if (!input->key_down[KEY_9]) input->key_just_down[KEY_9] = TRUE;
          input->key_down[KEY_9] = TRUE;
        }
        break;
        case SAPP_KEYCODE_ESCAPE:
        {
          if (!input->key_down[KEY_ESCAPE]) input->key_just_down[KEY_ESCAPE] = TRUE;
          input->key_down[KEY_ESCAPE] = TRUE;
        }
        break;
        case SAPP_KEYCODE_SPACE:
        {
          if (!input->key_down[KEY_SPACE]) input->key_just_down[KEY_SPACE] = TRUE;
          input->key_down[KEY_SPACE] = TRUE;
        }
        break;
        case SAPP_KEYCODE_ENTER:
        {
          if (!input->key_down[KEY_ENTER]) input->key_just_down[KEY_ENTER] = TRUE;
          input->key_down[KEY_ENTER] = TRUE;
        }
        break;
        case SAPP_KEYCODE_BACKSPACE:
        {
          if (!input->key_down[KEY_BACKSPACE]) input->key_just_down[KEY_BACKSPACE] = TRUE;
          input->key_down[KEY_BACKSPACE] = TRUE;
        }
        case SAPP_KEYCODE_TAB:
        {
          if (!input->key_down[KEY_TAB]) input->key_just_down[KEY_TAB] = TRUE;
          input->key_down[KEY_TAB] = TRUE;
        }
      }
      break;
    }
    case SAPP_EVENTTYPE_MOUSE_DOWN:
    {
      switch (event->mouse_button)
      {
        default: break;
        case SAPP_MOUSEBUTTON_LEFT:
        {
          if (!input->key_down[KEY_MOUSE_1]) input->key_just_down[KEY_MOUSE_1] = TRUE;
          input->key_down[KEY_MOUSE_1] = TRUE;
        }
        break;
        case SAPP_MOUSEBUTTON_RIGHT:
        {
          if (!input->key_down[KEY_MOUSE_2]) input->key_just_down[KEY_MOUSE_2] = TRUE;
          input->key_down[KEY_MOUSE_2] = TRUE;
        }
        break;
      }
    }
    break;
    case SAPP_EVENTTYPE_KEY_UP: 
    {
      switch (event->key_code)
      {
        default: break;
        case SAPP_KEYCODE_A:
        {
          if (input->key_down[KEY_A]) input->key_just_up[KEY_A] = TRUE;
          input->key_down[KEY_A] = FALSE;
        }
        break;
        case SAPP_KEYCODE_D:
        {
          if (input->key_down[KEY_D]) input->key_just_up[KEY_D] = TRUE;
          input->key_down[KEY_D] = FALSE;
        }
        break;
        case SAPP_KEYCODE_S:
        {
          if (input->key_down[KEY_S]) input->key_just_up[KEY_S] = TRUE;
          input->key_down[KEY_S] = FALSE;
        }
        break;
        case SAPP_KEYCODE_W: 
        {
          if (input->key_down[KEY_W]) input->key_just_up[KEY_W] = TRUE;
          input->key_down[KEY_W] = FALSE;
        }
        break;
        case SAPP_KEYCODE_0:
        {
          if (!input->key_down[KEY_0]) input->key_just_up[KEY_0] = TRUE;
          input->key_down[KEY_0] = FALSE;
        }
        break;
        case SAPP_KEYCODE_1:
        {
          if (!input->key_down[KEY_1]) input->key_just_up[KEY_1] = TRUE;
          input->key_down[KEY_1] = FALSE;
        }
        break;
        case SAPP_KEYCODE_2:
        {
          if (!input->key_down[KEY_2]) input->key_just_up[KEY_2] = TRUE;
          input->key_down[KEY_2] = FALSE;
        }
        break;
        case SAPP_KEYCODE_3:
        {
          if (!input->key_down[KEY_3]) input->key_just_up[KEY_3] = TRUE;
          input->key_down[KEY_3] = FALSE;
        }
        break;
        case SAPP_KEYCODE_9:
        {
          if (!input->key_down[KEY_9]) input->key_just_up[KEY_9] = TRUE;
          input->key_down[KEY_9] = FALSE;
        }
        break;
        case SAPP_KEYCODE_ESCAPE:
        {
          if (input->key_down[KEY_ESCAPE]) input->key_just_up[KEY_ESCAPE] = TRUE;
          input->key_down[KEY_ESCAPE] = FALSE;
        }
        break;
        case SAPP_KEYCODE_SPACE:
        {
          if (input->key_down[KEY_SPACE]) input->key_just_up[KEY_SPACE] = TRUE;
          input->key_down[KEY_SPACE] = FALSE;
        }
        break;
        case SAPP_KEYCODE_ENTER:
        {
          if (input->key_down[KEY_ENTER]) input->key_just_up[KEY_ENTER] = TRUE;
          input->key_down[KEY_ENTER] = FALSE;
        }
        break;
        case SAPP_KEYCODE_BACKSPACE:
        {
          if (input->key_down[KEY_ENTER]) input->key_just_up[KEY_ENTER] = TRUE;
          input->key_down[KEY_ENTER] = FALSE;
        }
        break;
        case SAPP_KEYCODE_TAB:
        {
          if (input->key_down[KEY_TAB]) input->key_just_up[KEY_TAB] = TRUE;
          input->key_down[KEY_TAB] = FALSE;
        }
      }
      break;
    }
    break;
    case SAPP_EVENTTYPE_MOUSE_UP:
    {
      switch (event->mouse_button)
      {
        default: break;
        case SAPP_MOUSEBUTTON_LEFT:
        {
          if (input->key_down[KEY_MOUSE_1]) input->key_just_up[KEY_MOUSE_1] = TRUE;
          input->key_down[KEY_MOUSE_1] = FALSE;
        }
        break;
        case SAPP_MOUSEBUTTON_RIGHT:
        {
          if (input->key_down[KEY_MOUSE_2]) input->key_just_up[KEY_MOUSE_2] = TRUE;
          input->key_down[KEY_MOUSE_2] = FALSE;
        }
        break;
      }
    }
    default: break;
  }
}
