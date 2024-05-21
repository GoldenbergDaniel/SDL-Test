#pragma once

#include "base/base_common.h"
#include "render/render.h"

#define D_SPRITE_SHEET_SIZE 16
#define D_SPRITE_SHEET_WIDTH 256
#define D_SPRITE_SHEET_HEIGHT 128
#define D_SPRITE_SHEET_COUNT_X 16
#define D_SPRITE_SHEET_COUNT_Y 8

#define D_SHADER_PRIMITIVE 0
#define D_SHADER_SPRITE 1

#define D_TEXTURE_SPRITE 0
#define D_TEXTURE_SCENE 1

#define D_SPRITE_COWBOY v2i(0, 0)
#define D_SPRITE_ZOMBIE v2i(1, 0)
#define D_SPRITE_GUN v2i(2, 0)
#define D_SPRITE_BULLET v2i(3, 0)

#define D_BLACK ((Vec4F) {0.01f, 0.01f, 0.01f, 1.0f})
#define D_WHITE ((Vec4F) {0.9f, 0.9f, 0.9f, 1.0f})
#define D_GRAY ((Vec4F) {0.5f, 0.5f, 0.5f, 1.0f})
#define D_RED ((Vec4F) {0.9f, 0.2f, 0.1f, 1.0f})
#define D_GREEN ((Vec4F) {0.3f, 0.9f, 0.2f, 1.0f})
#define D_BLUE ((Vec4F) {0.1f, 0.4f, 0.8f, 1.0f})
#define D_YELLOW ((Vec4F) {0.9f, 0.8f, 0.0f, 1.0f})

typedef struct Entity Entity; 

// @Assets ///////////////////////////////////////////////////////////////////////////////

#define TEXTURE_COUNT 8
#define SHADER_COUNT 8

typedef Vec2I TextureID;

typedef struct Resources Resources;
struct Resources
{
  R_Texture *textures;
  R_Shader *shaders;
};

Resources load_resources(Arena *arena, String path);
R_Shader *get_shader(u8 type);

// @Draw /////////////////////////////////////////////////////////////////////////////////

void clear_frame(Vec4F color);

void draw_rectangle(Vec2F pos, Vec2F dim, f32 rot, Vec4F tint);
void draw_rectangle_v(Vec3F p0, Vec3F p1, Vec3F p2, Vec3F p3, Vec4F tint);
void draw_rectangle_x(Mat3x3F xform, Vec4F tint);

void draw_sprite(Vec2F pos, Vec2F dim, f32 rot, Vec4F tint, TextureID tex, bool flash);
void draw_sprite_v(Vec3F p0, Vec3F p1, Vec3F p2, Vec3F p3, Vec4F tint, TextureID tex, bool flash);
void draw_sprite_x(Mat3x3F xform, Vec4F tint, TextureID tex_id, bool flash);

void draw_scene(Vec2F pos, Vec2F dim, Vec4F tint);
void draw_particles(Entity *en);
