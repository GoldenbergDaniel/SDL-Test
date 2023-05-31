#include <math.h>
#include <stdlib.h>

#include "globals.h"
#include "util.h"

inline
i32 round_f32(f32 num)
{
    return (i32) (num + 0.5f);
}

inline 
f32 lerp(f32 curr, f32 target, f32 rate)
{
    return (target - curr) * rate;
}

inline
f32 magnitude(v2 vec)
{
    return sqrtf(powf(vec.x, 2.0f) + powf(vec.y, 2.0f));
}

inline
v2 add_v2(v2 vec1, v2 vec2)
{
    return (v2) {
        vec1.x + vec2.x,
        vec1.y + vec2.y
    };
}

inline
f32 dot_v2(v2 vec1, v2 vec2)
{
    return (vec1.x * vec2.x) + (vec1.y * vec2.y);
}

v2 normalize_v2(v2 vec)
{
    f32 mag = magnitude(vec);
    
    return (v2) {
        vec.x / mag,
        vec.y / mag
    };
}

v2 get_screen_center(u16 w, u16 h)
{
    return (v2) {
        WINDOW_WIDTH/2.0f - w/2.0f,
        WINDOW_HEIGHT/2.0f - h/2.0f
    };
}

inline
v2 get_random_position(i32 min_x, i32 max_x, i32 min_y, i32 max_y)
{
    return (v2) {
        (rand() % max_x) + min_x,
        (rand() % max_y) + min_y
    };
}

inline
bool range_intersect(f32 min1, f32 max1, f32 min2, f32 max2)
{
    return (max1 >= min2) && (max2 >= min1);
}

bool rr_collision(v2 pos1, v2 pos2, f32 w1, f32 h1, f32 w2, f32 h2)
{
    return range_intersect(pos1.x, pos1.x+w1, pos2.x, pos2.x+w2) &&
           range_intersect(pos1.y, pos1.y+h1, pos2.y, pos2.y+h2);
}
