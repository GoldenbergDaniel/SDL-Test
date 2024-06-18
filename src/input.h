#pragma once

#include "sokol/sokol_app.h"

#include "base/base_common.h"

typedef enum KeyKind
{
  Key_A,
  Key_D,
  Key_P,
  Key_S,
  Key_W,
  Key_0,
  Key_1,
  Key_2,
  Key_3,
  Key_4,
  Key_5,
  Key_6,
  Key_7,
  Key_8,
  Key_9,
  Key_Space,
  Key_Escape,
  Key_Enter,
  Key_Backspace,
  Key_Tab,
  Key_Mouse1,
  Key_Mouse2,
  
  Key_COUNT,
} KeyKind;

typedef struct Input Input;
struct Input
{
  u8 keys[Key_COUNT];
  u8 keys_last[Key_COUNT];
  Vec2F mouse_pos;
};

bool is_key_pressed(KeyKind key);
bool is_key_just_pressed(KeyKind key);
bool is_key_released(KeyKind key);
Vec2F get_mouse_pos(void);
void remember_last_keys(void);
void handle_input_event(const sapp_event *event);
