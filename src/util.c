#include <math.h>

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

v2 add_v2(v2 vec1, v2 vec2)
{
    return (v2) {vec1.x+vec2.x, vec1.y+vec2.y};
}

v2 normalize(v2 vec)
{
    f32 mag = magnitude(vec);
    return (v2) {vec.x/mag, vec.y/mag};
}

v2 get_center(u16 width, u16 height)
{
    return (v2) {
        WINDOW_WIDTH/2.0f - width/2.0f,
        WINDOW_HEIGHT/2.0f - height/2.0f
    };
}
