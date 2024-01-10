#pragma once

#include "base/base_common.h"
#include "base/base_arena.h"
#include "base/base_string.h"
#include "base/base_math.h"
#include "render.h"

#define D_SPRITE_SHEET_SIZE 16
#define D_SPRITE_SHEET_WIDTH 256
#define D_SPRITE_SHEET_HEIGHT 128
#define D_SPRITE_SHEET_COUNT_X 16
#define D_SPRITE_SHEET_COUNT_Y 8

#define D_TEXTURE_COWBOY v2i(0,0)
#define D_TEXTURE_ZOMBIE v2i(1,0)
#define D_TEXTURE_GUN v2i(2,0)

#define D_BLACK ((Vec4F) {0.01f, 0.01f, 0.01f, 1.0f})
#define D_WHITE ((Vec4F) {0.9f, 0.9f, 0.9f, 1.0f})
#define D_GRAY ((Vec4F) {0.5f, 0.5f, 0.5f, 1.0f})
#define D_RED ((Vec4F) {0.9f, 0.2f, 0.1f, 1.0f})
#define D_GREEN ((Vec4F) {0.3f, 0.9f, 0.2f, 1.0f})
#define D_BLUE ((Vec4F) {0.1f, 0.4f, 0.8f, 1.0f})
#define D_YELLOW ((Vec4F) {0.9f, 0.8f, 0.0f, 1.0f})

typedef Vec2I D_TextureID;

typedef struct D_Resources D_Resources;
struct D_Resources
{
  R_Texture *textures;
  R_Shader *shaders;
};

// Assets ======================================================================================

#define D_TEXTURE_COUNT 1
#define D_SHADER_COUNT 2

D_Resources d_load_resources(Arena *arena, const String path);

// Renderer ====================================================================================

void d_clear(Vec4F color);
void d_draw_rectangle(Mat3x3F xform, Vec4F color);
void d_draw_sprite(Mat3x3F xform, Vec4F color, D_TextureID tex_id);
