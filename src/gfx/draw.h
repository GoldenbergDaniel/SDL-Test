#pragma once

#include "base/base_common.h"
#include "base/base_arena.h"
#include "base/base_math.h"
#include "gl_render.h"

typedef struct Renderer Renderer;
typedef struct RenderState RenderState;

struct RenderState
{
  R_Shader shader;
  R_Texture texture;
  R_VAO vao;
  u32 vbo;
  u32 ibo;
};

struct Renderer
{
  RenderState triangle;
  RenderState rectangle;
  RenderState sprite;
};

void d_init_renderer(Renderer *renderer);
void d_clear(Vec4F color);
void d_triangle(Mat3x3F xform, Vec4F color);
void d_rectangle(Mat3x3F xform, Vec4F color);
void d_sprite(Mat3x3F xform, Vec4F color, u16 texture_id);
