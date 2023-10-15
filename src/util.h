#pragma once

#include "base/base_common.h"
#include "draw.h"

// Timer =======================================================================================

typedef struct Timer Timer;
struct Timer
{
  f32 max_duration;
  f32 cugl_duration;
  bool is_running;
  bool should_tick;
  bool timeout;
};

void timer_start(Timer *timer, bool start_at_zero);
void timer_tick(Timer *timer, f64 dt);
// void timer_reset(Timer *timer);

// Input =======================================================================================

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

// Global ======================================================================================

typedef struct Global Global;
struct Global
{
  Input *input;
  D_Renderer *renderer;
};
