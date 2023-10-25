#include "base/base_common.h"
#include "component.h"

// @Timer ======================================================================================

void timer_start(Timer *timer, bool start_at_zero)
{
  timer->curr_duration = start_at_zero ? 0.0f : timer->start_duration;
  timer->ticking = TRUE;
  timer->timeout = FALSE;
  timer->should_tick = TRUE;
}

bool timer_tick(Timer *timer, f64 dt)
{
  if (timer->should_tick)
  {
    timer->curr_duration -= dt;

    if (timer->curr_duration <= 0.0f)
    {
      timer->timeout = TRUE;
      timer->ticking = FALSE;
    }
  }

  return timer->timeout;
}
