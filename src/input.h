#pragma once

#include "base/base_common.h"
#include "base/base_math.h"

typedef union SDL_Event SDL_Event;

typedef enum Key
{
  KEY_A,
  KEY_D,
  KEY_S,
  KEY_W,
  KEY_0,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_SPACE,
  KEY_ESCAPE,
  KEY_ENTER,
  KEY_BACKSPACE,
  _KEY_COUNT,
} Key;

typedef struct Input Input;
struct Input
{
  u8 key_down[_KEY_COUNT];
  u8 key_just_down[_KEY_COUNT];
  u8 key_just_up[_KEY_COUNT];
  Vec2I mouse_pos;
};

bool is_key_pressed(Key key);
bool is_key_just_pressed(Key key);
bool is_key_just_released(Key key);
Vec2F get_mouse_pos(void);
void clear_last_frame_input(void);
void handle_input(SDL_Event *event, bool *should_close);
