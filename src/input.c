#include "base/base.h"
#include "vecmath/vecmath.h"

#include "game.h"
#include "input.h"

#define SOKOL_NO_ENTRY
#include "sokol/sokol_app.h"
#include "sokol/sokol_time.h"

extern Globals global;

inline
bool is_key_pressed(InputKey key)
{
  return global.input.keys[key];
}

inline
bool is_key_just_pressed(InputKey key)
{
  return global.input.keys[key] && !global.input.keys_last[key];
}

inline
bool is_key_released(InputKey key)
{
  return !global.input.keys[key] && global.input.keys_last[key];
}

inline
Vec2F get_mouse_pos(void)
{
  return v2f(global.input.mouse_pos.x, global.window.height - global.input.mouse_pos.y);
}

void remember_last_keys(void)
{
  for (i32 i = 0; i < _KEY_COUNT; i++)
  {
    global.input.keys_last[i] = global.input.keys[i];
  }
}

void handle_input_event(const struct sapp_event *event)
{
  Input *input = &global.input;
  input->mouse_pos = v2f(event->mouse_x, event->mouse_y);

  switch (event->type)
  {
    case SAPP_EVENTTYPE_KEY_DOWN: 
    {
      switch (event->key_code)
      {
        default: break;
        case SAPP_KEYCODE_A: input->keys[KEY_A] = TRUE; break;
        case SAPP_KEYCODE_D: input->keys[KEY_D] = TRUE; break;
        case SAPP_KEYCODE_P: input->keys[KEY_P] = TRUE; break;
        case SAPP_KEYCODE_S: input->keys[KEY_S] = TRUE; break;
        case SAPP_KEYCODE_W: input->keys[KEY_W] = TRUE; break;
        case SAPP_KEYCODE_0: input->keys[KEY_0] = TRUE; break;
        case SAPP_KEYCODE_1: input->keys[KEY_1] = TRUE; break;
        case SAPP_KEYCODE_2: input->keys[KEY_2] = TRUE; break;
        case SAPP_KEYCODE_3: input->keys[KEY_3] = TRUE; break;
        case SAPP_KEYCODE_4: input->keys[KEY_4] = TRUE; break;
        case SAPP_KEYCODE_5: input->keys[KEY_5] = TRUE; break;
        case SAPP_KEYCODE_6: input->keys[KEY_6] = TRUE; break;
        case SAPP_KEYCODE_7: input->keys[KEY_7] = TRUE; break;
        case SAPP_KEYCODE_8: input->keys[KEY_8] = TRUE; break;
        case SAPP_KEYCODE_9: input->keys[KEY_9] = TRUE; break;
        case SAPP_KEYCODE_ESCAPE: input->keys[KEY_ESCAPE] = TRUE; break;
        case SAPP_KEYCODE_SPACE: input->keys[KEY_SPACE] = TRUE; break;
        case SAPP_KEYCODE_ENTER: input->keys[KEY_ENTER] = TRUE; break;
        case SAPP_KEYCODE_BACKSPACE: input->keys[KEY_BACKSPACE] = TRUE; break;
        case SAPP_KEYCODE_TAB: input->keys[KEY_TAB] = TRUE; break;
      }
      break;
    }
    case SAPP_EVENTTYPE_MOUSE_DOWN:
    {
      switch (event->mouse_button)
      {
        default: break;
        case SAPP_MOUSEBUTTON_LEFT: input->keys[KEY_MOUSE_1] = TRUE; break;
        case SAPP_MOUSEBUTTON_RIGHT: input->keys[KEY_MOUSE_2] = TRUE; break;
      }
    }
    break;
    case SAPP_EVENTTYPE_KEY_UP: 
    {
      switch (event->key_code)
      {
        default: break;
        case SAPP_KEYCODE_A: input->keys[KEY_A] = FALSE; break;
        case SAPP_KEYCODE_D: input->keys[KEY_D] = FALSE; break;
        case SAPP_KEYCODE_P: input->keys[KEY_P] = FALSE; break;
        case SAPP_KEYCODE_S: input->keys[KEY_S] = FALSE; break;
        case SAPP_KEYCODE_W: input->keys[KEY_W] = FALSE; break;
        case SAPP_KEYCODE_0: input->keys[KEY_0] = FALSE; break;
        case SAPP_KEYCODE_1: input->keys[KEY_1] = FALSE; break;
        case SAPP_KEYCODE_2: input->keys[KEY_2] = FALSE; break;
        case SAPP_KEYCODE_3: input->keys[KEY_3] = FALSE; break;
        case SAPP_KEYCODE_4: input->keys[KEY_4] = FALSE; break;
        case SAPP_KEYCODE_5: input->keys[KEY_5] = FALSE; break;
        case SAPP_KEYCODE_6: input->keys[KEY_6] = FALSE; break;
        case SAPP_KEYCODE_7: input->keys[KEY_7] = FALSE; break;
        case SAPP_KEYCODE_8: input->keys[KEY_8] = FALSE; break;
        case SAPP_KEYCODE_9: input->keys[KEY_9] = FALSE; break;
        case SAPP_KEYCODE_ESCAPE: input->keys[KEY_ESCAPE] = FALSE; break;
        case SAPP_KEYCODE_SPACE: input->keys[KEY_SPACE] = FALSE; break;
        case SAPP_KEYCODE_ENTER: input->keys[KEY_ENTER] = FALSE; break;
        case SAPP_KEYCODE_BACKSPACE: input->keys[KEY_ENTER] = FALSE; break;
        case SAPP_KEYCODE_TAB: input->keys[KEY_TAB] = FALSE; break;
      }
      break;
    }
    break;
    case SAPP_EVENTTYPE_MOUSE_UP:
    {
      switch (event->mouse_button)
      {
        default: break;
        case SAPP_MOUSEBUTTON_LEFT: input->keys[KEY_MOUSE_1] = FALSE; break;
        case SAPP_MOUSEBUTTON_RIGHT: input->keys[KEY_MOUSE_2] = FALSE; break;
      }
    }
    break;
    case SAPP_EVENTTYPE_RESIZED:
    {
      // NOTE(dg): Not sure why I removed this
      // for (i32 i = 0; i < _KEY_COUNT; i++)
      // {
      //   global.input.keys_last[i] = 0;
      //   global.input.keys[i] = 0;
      // }
    }
    break;
    default: break;
  }
}
