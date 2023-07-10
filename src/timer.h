#pragma once

typedef struct Timer Timer;
struct Timer
{
    f64 max_duration;
    f64 cur_duration;
    bool should_repeat;
};

bool timer_done(Timer timer);
