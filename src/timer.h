#pragma once

typedef struct Timer Timer;
struct Timer
{
  f64 max_duration;
  f64 cur_duration;
  bool is_done;
  bool is_repeating;
  bool is_running;
};

Timer timer_start(f64 duration, bool repeating);
void timer_tick(Timer *timer, f64 dt);
void timer_reset(Timer *timer);
void timer_stop(Timer *timer);
