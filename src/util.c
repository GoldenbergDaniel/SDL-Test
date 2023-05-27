#include "globals.h"

#include "util.h"

f32 lerp(f32 curr, f32 target, f32 rate)
{
    return (target - curr) * rate;
}

f32 magnitude(v2 vec)
{
    return sqrtf(powf(vec.x, 2.0f) + powf(vec.y, 2.0f));
}

v2 add_vec(v2 vec1, v2 vec2)
{
    return (v2) {vec1.x+vec2.x, vec1.y+vec2.y};
}

v2 normalize(v2 vec)
{
    f32 mag = magnitude(vec);
    
    return (v2) {vec.x/mag, vec.y/mag};
}

f64 time_in_seconds()
{
    return (f64) SDL_GetTicks64() * 0.001f;
}
