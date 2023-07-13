#include "globals.h"
#include "timer.h"

Timer timer_create(f64 duration, bool repeating)
{
  Timer timer;
  timer.max_duration = duration;
  timer.cur_duration = duration;
  timer.is_repeating = repeating;

  return timer;
}

void timer_stop(Timer *timer)
{

}

void timer_tick(Timer *timer, f64 dt)
{
  
}
