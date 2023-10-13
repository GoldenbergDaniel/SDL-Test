#pragma once

#include "base_common.h"

// Timer ====================================================================================

typedef struct Timer Timer;
struct Timer
{
  f32 max_duration;
  f32 cur_duration;
  bool is_running;
  bool should_tick;
  bool timeout;
};

void timer_start(Timer *timer, bool start_at_zero);
void timer_tick(Timer *timer, f64 dt);
// void timer_reset(Timer *timer);

// Input ====================================================================================

typedef struct Input Input;
struct Input
{
  u8 w;
  u8 a;
  u8 s;
  u8 d;
  u8 space;
  u8 escape;
};
