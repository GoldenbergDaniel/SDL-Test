#pragma once

#include "common.h"

// Timer ----------------------------------------------------------------------

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
