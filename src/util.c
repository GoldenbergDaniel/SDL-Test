#include <math.h>
#include <stdlib.h>

#include "globals.h"
#include "util.h"

inline 
f32 lerp(f32 curr, f32 target, f32 rate)
{
    return (target - curr) * rate;
}

inline
f32 magnitude_v2i(v2i vec)
{
    return sqrtf(powf(vec.x, 2.0f) + powf(vec.y, 2.0f));
}

inline
f32 magnitude_v2f(v2f vec)
{
    return sqrtf(powf(vec.x, 2.0f) + powf(vec.y, 2.0f));
}

inline
f32 distance_v2i(v2i vec1, v2i vec2)
{
    return sqrtf(powf(vec2.x - vec1.x, 2.0f) + powf(vec2.y - vec1.y, 2.0f));
}

inline
f32 distance_v2f(v2f vec1, v2f vec2)
{
    return sqrtf(powf(vec2.x - vec1.x, 2.0f) + powf(vec2.y - vec1.y, 2.0f));
}

inline
v2i add_v2i(v2i vec1, v2i vec2)
{
    return (v2i) {
        vec1.x + vec2.x,
        vec1.y + vec2.y
    };
}

inline
v2f add_v2f(v2f vec1, v2f vec2)
{
    return (v2f) {
        vec1.x + vec2.x,
        vec1.y + vec2.y
    };
}

inline
i32 dot_v2i(v2i vec1, v2i vec2)
{
    return (vec1.x * vec2.x) + (vec1.y * vec2.y);
}

inline
f32 dot_v2f(v2f vec1, v2f vec2)
{
    return (vec1.x * vec2.x) + (vec1.y * vec2.y);
}

v2f normalize_v2f(v2f vec)
{
    f32 mag = magnitude_v2f(vec);
    
    return (v2f) {
        vec.x / mag,
        vec.y / mag
    };
}

v2f get_random_position(u32 min_x, u32 max_x, u32 min_y, u32 max_y)
{
    return (v2f) {
        (rand() % max_x) + min_x,
        (rand() % max_y) + min_y
    };
}

inline
v2f get_screen_center(u16 w, u16 h)
{
    return (v2f) {
        (WINDOW_WIDTH / 2.0f) - (w / 2.0f),
        (WINDOW_HEIGHT / 2.0f) - (h / 2.0f)
    };
}

inline
v2f get_rect_center(v2f pos, u16 w, u16 h)
{
    return (v2f) {
        pos.x + (w / 2.0f),
        pos.y + (h / 2.0f)
    };
}

inline
bool range_intersect(f32 min1, f32 max1, f32 min2, f32 max2)
{
    return (max1 >= min2) && (max2 >= min1);
}

bool rr_collision(v2f pos1, v2f pos2, f32 w1, f32 h1, f32 w2, f32 h2)
{
    return range_intersect(pos1.x, pos1.x+w1, pos2.x, pos2.x+w2) &&
           range_intersect(pos1.y, pos1.y+h1, pos2.y, pos2.y+h2);
}
