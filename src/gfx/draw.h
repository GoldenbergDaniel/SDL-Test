#pragma once

#include "base/base_common.h"
#include "base/base_arena.h"
#include "base/base_math.h"
#include "gl_render.h"

typedef struct Renderer Renderer;
typedef struct DrawState DrawState;

struct DrawState
{
  R_Shader shader;
  R_GL_VAO vao;
  u32 vbo;
  u32 ibo;
};

struct Renderer
{
  DrawState triangle;
  DrawState rectangle;
};

void d_init_renderer(Renderer *renderer);
void d_clear(Vec4F color);
void d_triangle(Mat3x3F xform, Vec4F color);
void d_rectangle(Mat3x3F xform, Vec4F color);
