#include "globals.h"

#include "util.h"

f32 lerp(f32 curr, f32 target, f32 rate)
{
    return (target - curr) * rate;
}

f64 time_in_seconds()
{
    return (f64) SDL_GetTicks64() * 0.001f;
}
