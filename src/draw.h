#pragma once

#include "base/base_common.h"
#include "base/base_arena.h"
#include "base/base_math.h"
#include "render.h"

typedef struct D_Object D_Object;
struct D_Object
{
  R_Shader shader;
  R_Object vao;
  R_Object vbo;
  R_Object ibo;
  R_VertexLayout v_layout;
  R_VertexLayout c_layout;
};

typedef struct D_Renderer D_Renderer;
struct D_Renderer
{
  Mat3x3F camera;
  D_Object triangle;
  D_Object rectangle;
};

void d_init_renderer(D_Renderer *renderer);
void d_clear(Vec4F color);
void d_draw_triangle(Mat3x3F xform, Vec4F color);
void d_draw_rectangle(Mat3x3F xform, Vec4F color);
