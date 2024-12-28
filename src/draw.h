#pragma once

#include "base/base.h"
#include "render/render.h"
#include "ui/ui.h"

#define TEXTURE_SPRITE 0
#define TEXTURE_FONT 1
#define TEXTURE_SCENE 2

#define SPRITE_ATLAS_CELL 16
#define SPRITE_ATLAS_WIDTH 256
#define SPRITE_ATLAS_HEIGHT 256

#define GLYPH_ATLAS_CELL 8
#define GLYPH_ATLAS_WIDTH 128
#define GLYPH_ATLAS_HEIGHT 64

#define SHADER_PRIMITIVE 0
#define SHADER_SPRITE 1

#define DEBUG_BLACK ((Vec4F) {0.0f, 0.0f, 0.0f, 1.0f})
#define DEBUG_WHITE ((Vec4F) {1.0f, 1.0f, 1.0f, 1.0f})
#define DEBUG_GRAY ((Vec4F) {0.5f, 0.5f, 0.5f, 1.0f})
#define DEBUG_RED ((Vec4F) {0.9f, 0.2f, 0.1f, 1.0f})
#define DEBUG_GREEN ((Vec4F) {0.3f, 0.9f, 0.2f, 1.0f})
#define DEBUG_BLUE ((Vec4F) {0.1f, 0.4f, 0.8f, 1.0f})
#define DEBUG_YELLOW ((Vec4F) {0.9f, 0.8f, 0.0f, 1.0f})

typedef struct Entity Entity;

// @Assets ///////////////////////////////////////////////////////////////////////////////

#define TEXTURE_COUNT 8
#define SHADER_COUNT 8

typedef struct Sprite Sprite;
struct Sprite
{
  Vec2I coord;
  Vec2I grid;
};

typedef struct Resources Resources;
struct Resources
{
  R_Texture *textures;
  R_Shader *shaders;
};

Resources load_resources(Arena *arena, String path);
R_Shader *get_shader(u8 type);

UI_Glyph get_glyph(char glyph);

// @Draw /////////////////////////////////////////////////////////////////////////////////

void clear_frame(Vec4F color);

void draw_rect(Vec2F pos, Vec2F dim, f32 rot, Vec4F tint);
void draw_rect_v(Vec3F p0, Vec3F p1, Vec3F p2, Vec3F p3, Vec4F tint);
void draw_rect_x(Mat3x3F xform, Vec4F tint);

void draw_sprite(Vec2F pos, Vec2F dim, f32 rot, Vec4F tint, Sprite sprite, bool flash);
void draw_sprite_v(Vec3F p0, Vec3F p1, Vec3F p2, Vec3F p3, Vec4F tint, Sprite sprite, bool flash);
void draw_sprite_x(Mat3x3F xform, Vec2F dim, Vec4F tint, Sprite sprite, bool flash);

void draw_scene(Vec2F pos, Vec2F dim, Vec4F tint);
void draw_glyph(Vec2F pos, f32 size, Vec4F tint, Vec2I tex_coord);
