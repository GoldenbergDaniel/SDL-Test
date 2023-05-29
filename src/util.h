#pragma once

#define UTIL_H

#define len(arr) sizeof(arr)/sizeof(arr[0])

typedef struct v2
{
    f32 x;
    f32 y;
} v2;

typedef struct vi2
{
    i32 x;
    i32 y;
} vi2;

f32 lerp(f32 curr, f32 target, f32 rate);
f32 magnitude(v2 vec);
v2 add_v2(v2 vec1, v2 vec2);
v2 normalize(v2 vec);
v2 get_center(u16 width, u16 height);
