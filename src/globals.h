#pragma once

#include "base/base_common.h"
#include "render/render.h"
#include "input.h"
#include "draw.h"

#ifdef _WIN32
#define TIME_STEP (1.0f / 120)
#define ANIM_TICK 2
#else
#define TIME_STEP (1.0f / 60)
#define ANIM_TICK 1
#endif

#define WIDTH 960.0f
#define HEIGHT 540.0f
#define SPRITE_SCALE 5

typedef struct Globals Globals;
struct Globals
{
  Input input;
  Resources resources;
  R_Renderer renderer;
  Vec2F window;
  Vec4F viewport;
  bool debug;

  struct
  {
    f64 current_time;
    f64 elapsed_time;
    f64 accumulator;
  } frame;
};

f32 get_width(void);
f32 get_height(void);
