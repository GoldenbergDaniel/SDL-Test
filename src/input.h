#pragma once

#include "base/base_inc.h"

struct sapp_event;

#include "base/base_inc.h"
#include "sokol/sokol_app.h"

typedef enum InputKey
{
  KEY_A,
  KEY_D,
  KEY_S,
  KEY_W,
  KEY_0,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_9,
  KEY_SPACE,
  KEY_ESCAPE,
  KEY_ENTER,
  KEY_BACKSPACE,
  KEY_TAB,
  KEY_MOUSE_1,
  KEY_MOUSE_2,
  
  _KEY_COUNT,
} InputKey;

typedef struct Input Input;
struct Input
{
  u8 key_down[_KEY_COUNT];
  u8 key_just_down[_KEY_COUNT];
  u8 key_just_up[_KEY_COUNT];
  Vec2F mouse_pos;
};

bool is_key_pressed(InputKey key);
bool is_key_just_pressed(InputKey key);
bool is_key_just_released(InputKey key);
Vec2F get_mouse_pos(void);
void clear_last_frame_input(void);
void handle_input_event(const sapp_event *event);
