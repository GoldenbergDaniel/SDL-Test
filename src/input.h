#pragma once

#include "sokol/sokol_app.h"

#include "base/base_common.h"

typedef enum InputKey
{
  KEY_A,
  KEY_D,
  KEY_P,
  KEY_S,
  KEY_W,
  KEY_0,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
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
  u8 keys[_KEY_COUNT];
  u8 keys_last[_KEY_COUNT];
  Vec2F mouse_pos;
};

bool is_key_pressed(InputKey key);
bool is_key_just_pressed(InputKey key);
bool is_key_released(InputKey key);
Vec2F get_mouse_pos(void);
void remember_last_keys(void);
void handle_input_event(const sapp_event *event);
