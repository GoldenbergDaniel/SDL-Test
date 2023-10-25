#pragma once

#include "base/base_common.h"
#include "base/base_arena.h"
#include "base/base_math.h"
#include "gl_render.h"

typedef struct D_Object D_Object;
struct D_Object
{
  R_Shader shader;
  R_GL_VAO vao;
  u32 vbo;
  u32 ibo;
};

typedef struct D_Renderer D_Renderer;
struct D_Renderer
{
  Mat3x3F *camera;
  D_Object triangle;
  D_Object rectangle;
};

void d_init_renderer(D_Renderer *renderer);
void d_clear(Vec4F color);
void d_triangle(Mat3x3F xform, Vec4F color);
void d_rectangle(Mat3x3F xform, Vec4F color);
