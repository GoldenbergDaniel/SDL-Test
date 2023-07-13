#include "globals.h"
#include "component.h"

// Timer ----------------------------------------------------------------------

Timer timer_start(f32 duration)
{
  Timer timer;
  timer.max_duration = duration;
  timer.cur_duration = duration;
  timer.is_running = TRUE;

  return timer;
}

bool timer_tick(Timer *timer, f64 dt)
{
  if (timer->cur_duration > 0)
  {
    timer->cur_duration -= dt;
    return FALSE;
  }
  else
  {
    timer->cur_duration = 0;
    timer->is_running = FALSE;
    return TRUE;
  }
}
