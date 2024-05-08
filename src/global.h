#pragma once

#include "base/base_inc.h"

#include "render/render.h"
#include "draw/draw.h"
#include "input/input.h"

#define WIDTH 1270
#define HEIGHT 740

#define SPRITE_SCALE ((Vec2F) {6, 6})

typedef struct Global Global;
struct Global
{
  InputState input;
  D_Resources resources;
  R_Renderer renderer;

  bool debug;
};

Vec2F screen_to_world(Vec2F pos);
