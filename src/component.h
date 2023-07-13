#pragma once

// Timer ----------------------------------------------------------------------

typedef struct Timer Timer;
struct Timer
{
  f32 max_duration;
  f32 cur_duration;
  bool is_running;
};

Timer timer_start(f32 duration);
bool timer_tick(Timer *timer, f64 dt);
void timer_reset(Timer *timer);
