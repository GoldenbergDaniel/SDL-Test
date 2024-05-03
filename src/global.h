#pragma once

#include "base/base_inc.h"

#include "render/render.h"
#include "draw/draw.h"
#include "input/input.h"

#define WIDTH 1270
#define HEIGHT 740

#define SPRITE_SCALE (Vec2F) {7, 7}

typedef struct Global Global;
struct Global
{
  Input input;
  D_Resources resources;
  R_Renderer renderer;
};

Vec2F screen_to_world(Vec2F pos);
