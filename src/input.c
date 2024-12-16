#include "base/base.h"
#include "vecmath/vecmath.h"

#include "game.h"
#include "input.h"

#define SOKOL_NO_ENTRY
#include "sokol/sokol_app.h"

extern Globals global;

inline
bool is_key_pressed(KeyKind key)
{
  return global.input.keys[key];
}

inline
bool is_key_just_pressed(KeyKind key)
{
  return global.input.keys[key] && !global.input.keys_last[key];
}

inline
bool is_key_released(KeyKind key)
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
  for (i32 i = 0; i < Key_COUNT; i++)
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
      case SAPP_KEYCODE_A: input->keys[Key_A] = TRUE; break;
      case SAPP_KEYCODE_D: input->keys[Key_D] = TRUE; break;
      case SAPP_KEYCODE_P: input->keys[Key_P] = TRUE; break;
      case SAPP_KEYCODE_R: input->keys[Key_R] = TRUE; break;
      case SAPP_KEYCODE_S: input->keys[Key_S] = TRUE; break;
      case SAPP_KEYCODE_W: input->keys[Key_W] = TRUE; break;
      case SAPP_KEYCODE_0: input->keys[Key_0] = TRUE; break;
      case SAPP_KEYCODE_1: input->keys[Key_1] = TRUE; break;
      case SAPP_KEYCODE_2: input->keys[Key_2] = TRUE; break;
      case SAPP_KEYCODE_3: input->keys[Key_3] = TRUE; break;
      case SAPP_KEYCODE_4: input->keys[Key_4] = TRUE; break;
      case SAPP_KEYCODE_5: input->keys[Key_5] = TRUE; break;
      case SAPP_KEYCODE_6: input->keys[Key_6] = TRUE; break;
      case SAPP_KEYCODE_7: input->keys[Key_7] = TRUE; break;
      case SAPP_KEYCODE_8: input->keys[Key_8] = TRUE; break;
      case SAPP_KEYCODE_9: input->keys[Key_9] = TRUE; break;
      case SAPP_KEYCODE_ESCAPE: input->keys[Key_Escape] = TRUE; break;
      case SAPP_KEYCODE_SPACE: input->keys[Key_Space] = TRUE; break;
      case SAPP_KEYCODE_ENTER: input->keys[Key_Enter] = TRUE; break;
      case SAPP_KEYCODE_BACKSPACE: input->keys[Key_Backspace] = TRUE; break;
      case SAPP_KEYCODE_TAB: input->keys[Key_Tab] = TRUE; break;
      }
      break;
    }
    case SAPP_EVENTTYPE_MOUSE_DOWN:
    {
      switch (event->mouse_button)
      {
      default: break;
      case SAPP_MOUSEBUTTON_LEFT: input->keys[Key_Mouse1] = TRUE; break;
      case SAPP_MOUSEBUTTON_RIGHT: input->keys[Key_Mouse2] = TRUE; break;
      }
    }
    break;
    case SAPP_EVENTTYPE_KEY_UP: 
    {
      switch (event->key_code)
      {
      default: break;
      case SAPP_KEYCODE_A: input->keys[Key_A] = FALSE; break;
      case SAPP_KEYCODE_D: input->keys[Key_D] = FALSE; break;
      case SAPP_KEYCODE_P: input->keys[Key_P] = FALSE; break;
      case SAPP_KEYCODE_R: input->keys[Key_R] = FALSE; break;
      case SAPP_KEYCODE_S: input->keys[Key_S] = FALSE; break;
      case SAPP_KEYCODE_W: input->keys[Key_W] = FALSE; break;
      case SAPP_KEYCODE_0: input->keys[Key_0] = FALSE; break;
      case SAPP_KEYCODE_1: input->keys[Key_1] = FALSE; break;
      case SAPP_KEYCODE_2: input->keys[Key_2] = FALSE; break;
      case SAPP_KEYCODE_3: input->keys[Key_3] = FALSE; break;
      case SAPP_KEYCODE_4: input->keys[Key_4] = FALSE; break;
      case SAPP_KEYCODE_5: input->keys[Key_5] = FALSE; break;
      case SAPP_KEYCODE_6: input->keys[Key_6] = FALSE; break;
      case SAPP_KEYCODE_7: input->keys[Key_7] = FALSE; break;
      case SAPP_KEYCODE_8: input->keys[Key_8] = FALSE; break;
      case SAPP_KEYCODE_9: input->keys[Key_9] = FALSE; break;
      case SAPP_KEYCODE_ESCAPE: input->keys[Key_Escape] = FALSE; break;
      case SAPP_KEYCODE_SPACE: input->keys[Key_Space] = FALSE; break;
      case SAPP_KEYCODE_ENTER: input->keys[Key_Enter] = FALSE; break;
      case SAPP_KEYCODE_BACKSPACE: input->keys[Key_Backspace] = FALSE; break;
      case SAPP_KEYCODE_TAB: input->keys[Key_Tab] = FALSE; break;
      }
      break;
    }
    break;
    case SAPP_EVENTTYPE_MOUSE_UP:
    {
      switch (event->mouse_button)
      {
      default: break;
      case SAPP_MOUSEBUTTON_LEFT: input->keys[Key_Mouse1] = FALSE; break;
      case SAPP_MOUSEBUTTON_RIGHT: input->keys[Key_Mouse2] = FALSE; break;
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
