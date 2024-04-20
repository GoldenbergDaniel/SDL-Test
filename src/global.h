#pragma once

#include "base/base_inc.h"

#include "render/render.h"
#include "draw/draw.h"
#include "input.h"

#define WIDTH 1024
#define HEIGHT 640

typedef struct Global Global;
struct Global
{
  Input input;
  D_Resources resources;
  R_Renderer renderer;
};

Vec2F screen_to_world(Vec2F pos);
