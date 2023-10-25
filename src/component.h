#pragma once

#include "base/base_common.h"

// @Timer ======================================================================================

typedef struct Timer Timer;
struct Timer
{
  f64 start_duration;
  f64 curr_duration;
  bool should_tick;
  bool ticking;
  bool timeout;
  bool loop;
};

void timer_start(Timer *timer, bool start_at_zero);
bool timer_tick(Timer *timer, f64 dt);
