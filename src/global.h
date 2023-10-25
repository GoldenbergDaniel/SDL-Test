#pragma once

#include "base/base_common.h"
#include "gfx/draw.h"

// @Input ======================================================================================

typedef struct Input Input;
struct Input
{
  u8 w;
  u8 a;
  u8 s;
  u8 d;
  u8 space;
  u8 escape;
  i32 mouse_x;
  i32 mouse_y;
};

// @Global =====================================================================================

typedef struct Global Global;
struct Global
{
  Input *input;
  D_Renderer *renderer;
};
