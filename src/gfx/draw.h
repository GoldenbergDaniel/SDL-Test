#pragma once

#include "base/base_common.h"
#include "base/base_arena.h"
#include "base/base_string.h"
#include "base/base_math.h"
#include "gl_render.h"

typedef struct D_Assets D_Assets;
typedef struct D_Renderer D_Renderer;
typedef struct D_RenderState D_RenderState;

struct D_Assets
{
  R_Texture *textures;
  R_Shader *shaders;
};

struct D_RenderState
{
  R_Shader shader;
  R_Texture texture;
  R_VAO vao;
  u32 vbo;
  u32 ibo;
};

struct D_Renderer
{
  D_RenderState triangle;
  D_RenderState rectangle;
  D_RenderState sprite;
};

// Assets ======================================================================================

#define TEXTURE_COUNT 16
#define SHADER_COUNT 16

D_Assets d_load_assets(Arena *arena, const String path);
R_Texture d_get_texture(const String name);
R_Shader d_get_shader(const String name);

// Renderer ====================================================================================

D_Renderer d_create_renderer(void);
void d_clear(Vec4F color);
void d_triangle(Mat3x3F xform, Vec4F color);
void d_rectangle(Mat3x3F xform, Vec4F color);
void d_sprite(Mat3x3F xform, Vec4F color, u16 texture_id);
