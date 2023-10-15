#include "base/base_common.h"
#include "util.h"

// Timer ----------------------------------------------------------------------

void timer_start(Timer *timer, bool start_at_zero)
{
  timer->cugl_duration = start_at_zero ? 0.0f : timer->max_duration;
  timer->is_running = TRUE;
  timer->timeout = FALSE;
}

void timer_tick(Timer *timer, f64 dt)
{
  timer->cugl_duration -= dt;

  if (timer->cugl_duration <= 0.0f)
  {
    timer->cugl_duration = 0.0f;
    timer->is_running = FALSE;
    timer->timeout = TRUE;
  }
}
