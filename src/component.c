#include "common.h"
#include "component.h"

// Timer ----------------------------------------------------------------------

void timer_start(Timer *timer, bool start_at_zero)
{
  timer->cur_duration = start_at_zero ? 0.0f : timer->max_duration;
  timer->is_running = TRUE;
  timer->timeout = FALSE;
}

void timer_tick(Timer *timer, f64 dt)
{
  timer->cur_duration -= dt;

  if (timer->cur_duration <= 0.0f)
  {
    timer->cur_duration = 0.0f;
    timer->is_running = FALSE;
    timer->timeout = TRUE;
  }
}
