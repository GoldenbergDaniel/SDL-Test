#include "base/base_common.h"

#include "global.h"

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
Vec2I mouse_position(void)
{
  return GLOBAL->input->mouse_pos;
}

void clear_last_frame_input(void)
{
  for (u8 i = 0; i < sizeof (GLOBAL->input->key_just_down); i++)
  {
    GLOBAL->input->key_just_down[i] = FALSE;
    GLOBAL->input->key_just_up[i] = FALSE;
  }
}
