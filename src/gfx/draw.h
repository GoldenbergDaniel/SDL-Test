#pragma once

#include "base/base_common.h"
#include "base/base_arena.h"
#include "base/base_string.h"
#include "base/base_math.h"
#include "gl_render.h"

#define D_SPRITE_SHEET_SIZE 16
#define D_SPRITE_SHEET_WIDTH 256
#define D_SPRITE_SHEET_HEIGHT 128
#define D_SPRITE_SHEET_COUNT_X 16
#define D_SPRITE_SHEET_COUNT_Y 8

#define D_TEXTURE_PLAYER v2i(0,0)
#define D_TEXTURE_ALIEN v2i(1,0)
#define D_TEXTURE_GUN v2i(2,0)

#define D_BLACK ((Vec4F) {0.01f, 0.01f, 0.01f, 1.0f})
#define D_WHITE ((Vec4F) {0.9f, 0.9f, 0.9f, 1.0f})
#define D_GRAY ((Vec4F) {0.5f, 0.5f, 0.5f, 1.0f})
#define D_RED ((Vec4F) {0.9f, 0.2f, 0.1f, 1.0f})
#define D_GREEN ((Vec4F) {0.3f, 0.9f, 0.2f, 1.0f})
#define D_BLUE ((Vec4F) {0.1f, 0.4f, 0.8f, 1.0f})
#define D_YELLOW ((Vec4F) {0.9f, 0.8f, 0.0f, 1.0f})

typedef struct D_Resources D_Resources;
typedef struct D_Renderer D_Renderer;
typedef struct D_RenderState D_RenderState;

typedef Vec2I D_TextureID;

struct D_Resources
{
  R_Texture *textures;
  R_Shader *shaders;
};

struct D_RenderState
{
  R_Shader shader;
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

// typedef struct D_RenderParams D_RenderParams;
// struct D_RenderParams
// {
//   Vec2F xform;
//   Vec4F color;
//   f32 rot;
// };

// Assets ======================================================================================

#define D_TEXTURE_COUNT 16
#define D_SHADER_COUNT 16

D_Resources d_load_resources(Arena *arena, const String path);
R_Texture d_get_texture(const String name);
R_Shader d_get_shader(const String name);

// Renderer ====================================================================================

// #define d_sprite(...) d_sprite_(&(D_RenderParams){.xform = m3x3f(1)), .color = v4f(1, 1, 1, 1), .rot = 0, __VA_ARGS__})

D_Renderer d_create_renderer(void);
void d_clear(Vec4F color);
void d_triangle(Mat3x3F xform, Vec4F color);
void d_rectangle(Mat3x3F xform, Vec4F color);
void d_sprite(Mat3x3F xform, Vec4F color, D_TextureID tex_id);
