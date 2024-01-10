#include "glad/glad.h"
#include "base/base_common.h"
#include "base/base_arena.h"

#include "../game.h"
#include "../global.h"
#include "render.h"
#include "shaders.h"
#include "draw.h"

extern Global *GLOBAL;

// Assets ======================================================================================

D_Resources d_load_resources(Arena *arena, const String path)
{
  D_Resources res = {0};
  res.textures = arena_alloc(arena, sizeof (R_Texture) * D_TEXTURE_COUNT);
  res.shaders = arena_alloc(arena, sizeof (R_Shader) * D_SHADER_COUNT);

  R_Shader primitive_shader = r_create_shader(primitive_vert_src, primitive_frag_src);
  res.shaders[0] = primitive_shader;

  R_Shader sprite_shader = r_create_shader(sprite_vert_src, sprite_frag_src);
  res.shaders[1] = sprite_shader;

#ifdef DEBUG
  char *sprites_path = "../res/texture/sprites.png";
#else
  char *sprites_path = "res/texture/sprites.png";
#endif

  R_Texture sprites = r_create_texture(sprites_path);

  res.textures[0] = sprites;

  return res;
}

// D_Renderer ====================================================================================

#include <stdio.h>

inline
void d_clear(Vec4F color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void d_draw_rectangle(Mat3x3F xform, Vec4F tint)
{
  Vec3F p0 = transform_3f(v3f(-5.0f,  5.0f, 1.0f), xform);
  Vec3F p1 = transform_3f(v3f( 5.0f,  5.0f, 1.0f), xform);
  Vec3F p2 = transform_3f(v3f( 5.0f, -5.0f, 1.0f), xform);
  Vec3F p3 = transform_3f(v3f(-5.0f, -5.0f, 1.0f), xform);

  R_Renderer *renderer = &GLOBAL->renderer;
  r_push_vertex(renderer, v4f(p0.x, p0.y, p0.z, 0.0f), tint, V4F_ZERO);
  r_push_vertex(renderer, v4f(p1.x, p1.y, p1.z, 0.0f), tint, V4F_ZERO);
  r_push_vertex(renderer, v4f(p2.x, p2.y, p2.z, 0.0f), tint, V4F_ZERO);
  r_push_vertex(renderer, v4f(p3.x, p3.y, p3.z, 0.0f), tint, V4F_ZERO);
  r_push_quad_indices(renderer);
}

void d_draw_sprite(Mat3x3F xform, Vec4F tint, D_TextureID tex_id)
{
  Vec3F p0 = transform_3f(v3f(-5.0f,  5.0f, 1.0f), xform);
  Vec3F p1 = transform_3f(v3f( 5.0f,  5.0f, 1.0f), xform);
  Vec3F p2 = transform_3f(v3f( 5.0f, -5.0f, 1.0f), xform);
  Vec3F p3 = transform_3f(v3f(-5.0f, -5.0f, 1.0f), xform);

  tex_id.y = D_SPRITE_SHEET_COUNT_Y - tex_id.y - 1;

  const Vec4F top_left =
  {
    ((f32) tex_id.x * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_WIDTH, 
    ((f32) (tex_id.y+1) * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_HEIGHT
  };

  const Vec4F top_right =
  {
    ((f32) (tex_id.x+1) * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_WIDTH, 
    ((f32) (tex_id.y+1) * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_HEIGHT
  };

 const Vec4F bot_right =
  {
    ((f32) (tex_id.x+1) * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_WIDTH, 
    ((f32) tex_id.y * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_HEIGHT
  };

  const Vec4F bot_left =
  {
    ((f32) tex_id.x * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_WIDTH, 
    ((f32) tex_id.y * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_HEIGHT
  };

  R_Renderer *renderer = &GLOBAL->renderer;
  r_push_vertex(renderer, v4f(p0.x, p0.y, p0.z, 0.0f), tint, top_left);
  r_push_vertex(renderer, v4f(p1.x, p1.y, p1.z, 0.0f), tint, top_right);
  r_push_vertex(renderer, v4f(p2.x, p2.y, p2.z, 0.0f), tint, bot_right);
  r_push_vertex(renderer, v4f(p3.x, p3.y, p3.z, 0.0f), tint, bot_left);
  r_push_quad_indices(renderer);
}
