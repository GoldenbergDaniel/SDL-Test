#include "glad/glad.h"

#include "base/base_inc.h"
#include "render/render.h"
#include "render/shaders.h"
#include "global.h"
#include "entity.h"
#include "draw.h"

extern Global *GLOBAL;

// @Assets ///////////////////////////////////////////////////////////////////////////////

Resources load_resources(Arena *arena, String path)
{
  Resources res = {0};
  res.textures = arena_alloc(arena, sizeof (R_Texture) * TEXTURE_COUNT);
  res.shaders = arena_alloc(arena, sizeof (R_Shader) * SHADER_COUNT);

  R_Shader primitive_shader = r_create_shader(primitive_VERT_SRC, primitive_FRAG_SRC);
  res.shaders[0] = primitive_shader;

  R_Shader sprite_shader = r_create_shader(sprite_VERT_SRC, sprite_FRAG_SRC);
  res.shaders[1] = sprite_shader;

  Arena scratch = arena_get_scratch(arena);
  String path_to_sprites = str_concat(path, str("/texture/sprites.png"), &scratch);
  R_Texture sprites = r_create_texture(path_to_sprites);
  arena_clear(&scratch);

  res.textures[0] = sprites;

  return res;
}

R_Shader *get_shader(u8 type)
{
  return &GLOBAL->resources.shaders[type];
}

// @Draw /////////////////////////////////////////////////////////////////////////////////

inline
void clear_frame(Vec4F color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void draw_rectangle(Vec2F pos, Vec2F dim, f32 rot, Vec4F tint)
{
  R_Renderer *renderer = &GLOBAL->renderer;
  r_use_shader(renderer, &GLOBAL->resources.shaders[D_SHADER_PRIMITIVE]);

  Mat3x3F xform = m3x3f(1.0f);
  xform = mul_3x3f(rotate_3x3f(rot), xform);
  xform = mul_3x3f(translate_3x3f(pos.x, pos.y), xform);
  xform = mul_3x3f(renderer->projection, xform);
  
  Vec3F p0 = transform_3f(v3f(0.0f, dim.height, 1.0f), xform);
  Vec3F p1 = transform_3f(v3f(dim.width, dim.height, 1.0f), xform);
  Vec3F p2 = transform_3f(v3f(dim.width, 0.0f, 1.0f), xform);
  Vec3F p3 = transform_3f(v3f(0.0f, 0.0f, 1.0f), xform);

  r_push_vertex(renderer, v4f(p0.x, p0.y, p0.z, 0.0f), tint, V4F_ZERO);
  r_push_vertex(renderer, v4f(p1.x, p1.y, p1.z, 0.0f), tint, V4F_ZERO);
  r_push_vertex(renderer, v4f(p2.x, p2.y, p2.z, 0.0f), tint, V4F_ZERO);
  r_push_vertex(renderer, v4f(p3.x, p3.y, p3.z, 0.0f), tint, V4F_ZERO);
  r_push_quad_indices(renderer);
}

void draw_rectangle_v(Vec3F p0, Vec3F p1, Vec3F p2, Vec3F p3, Vec4F tint)
{
  R_Renderer *renderer = &GLOBAL->renderer;
  r_use_shader(renderer, &GLOBAL->resources.shaders[D_SHADER_PRIMITIVE]);

  r_push_vertex(renderer, v4f(p0.x, p0.y, p0.z, 0.0f), tint, V4F_ZERO);
  r_push_vertex(renderer, v4f(p1.x, p1.y, p1.z, 0.0f), tint, V4F_ZERO);
  r_push_vertex(renderer, v4f(p2.x, p2.y, p2.z, 0.0f), tint, V4F_ZERO);
  r_push_vertex(renderer, v4f(p3.x, p3.y, p3.z, 0.0f), tint, V4F_ZERO);
  r_push_quad_indices(renderer);
}

void draw_rectangle_x(Mat3x3F xform, Vec4F tint)
{
  R_Renderer *renderer = &GLOBAL->renderer;
  r_use_shader(renderer, &GLOBAL->resources.shaders[D_SHADER_PRIMITIVE]);

  Vec3F p0 = transform_3f(v3f( 0.0f, 10.0f, 1.0f), xform);
  Vec3F p1 = transform_3f(v3f(10.0f, 10.0f, 1.0f), xform);
  Vec3F p2 = transform_3f(v3f(10.0f, 0.0f, 1.0f), xform);
  Vec3F p3 = transform_3f(v3f( 0.0f, 0.0f, 1.0f), xform);

  r_push_vertex(renderer, v4f(p0.x, p0.y, p0.z, 0.0f), tint, V4F_ZERO);
  r_push_vertex(renderer, v4f(p1.x, p1.y, p1.z, 0.0f), tint, V4F_ZERO);
  r_push_vertex(renderer, v4f(p2.x, p2.y, p2.z, 0.0f), tint, V4F_ZERO);
  r_push_vertex(renderer, v4f(p3.x, p3.y, p3.z, 0.0f), tint, V4F_ZERO);
  r_push_quad_indices(renderer);
}

void draw_sprite(Vec2F pos, Vec2F dim, f32 rot, Vec2F off, Vec4F tint, TextureID tex)
{
  R_Renderer *renderer = &GLOBAL->renderer;
  r_use_texture(renderer, &GLOBAL->resources.textures[D_TEXTURE_SPRITE]);
  r_use_shader(renderer, &GLOBAL->resources.shaders[D_SHADER_SPRITE]);

  Mat3x3F xform = m3x3f(1.0f);
  // xform = mul_3x3f(translate_3x3f(off.x, off.y), xform);
  xform = mul_3x3f(scale_3x3f(dim.x, dim.y), xform);
  xform = mul_3x3f(rotate_3x3f(rot * RADIANS), xform);
  xform = mul_3x3f(translate_3x3f(pos.x, pos.y), xform);
  xform = mul_3x3f(renderer->projection, xform);
  
  Vec3F p0 = transform_3f(v3f(0.0f, 1.0f, 1.0f), xform); // tl
  Vec3F p1 = transform_3f(v3f(1.0f, 1.0f, 1.0f), xform); // tr
  Vec3F p2 = transform_3f(v3f(1.0f, 0.0f, 1.0f), xform); // br
  Vec3F p3 = transform_3f(v3f(0.0f, 0.0f, 1.0f), xform); // bl

  tex.y = D_SPRITE_SHEET_COUNT_Y - tex.y - 1;

  const Vec4F top_left = {
    ((f32) tex.x * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_WIDTH, 
    ((f32) (tex.y+1) * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_HEIGHT
  };

  const Vec4F top_right = {
    ((f32) (tex.x+1) * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_WIDTH, 
    ((f32) (tex.y+1) * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_HEIGHT
  };

  const Vec4F bot_right = {
    ((f32) (tex.x+1) * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_WIDTH, 
    ((f32) tex.y * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_HEIGHT
  };

  const Vec4F bot_left = {
    ((f32) tex.x * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_WIDTH, 
    ((f32) tex.y * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_HEIGHT
  };

  r_push_vertex(renderer, v4f(p0.x, p0.y, p0.z, 0.0f), tint, top_left);
  r_push_vertex(renderer, v4f(p1.x, p1.y, p1.z, 0.0f), tint, top_right);
  r_push_vertex(renderer, v4f(p2.x, p2.y, p2.z, 0.0f), tint, bot_right);
  r_push_vertex(renderer, v4f(p3.x, p3.y, p3.z, 0.0f), tint, bot_left);
  r_push_quad_indices(renderer);
}

void draw_sprite_v(Vec3F p0, Vec3F p1, Vec3F p2, Vec3F p3, Vec4F tint, TextureID tex)
{
  R_Renderer *renderer = &GLOBAL->renderer;
  r_use_texture(renderer, &GLOBAL->resources.textures[D_TEXTURE_SPRITE]);
  r_use_shader(renderer, &GLOBAL->resources.shaders[D_SHADER_SPRITE]);

  tex.y = D_SPRITE_SHEET_COUNT_Y - tex.y - 1;

  const Vec4F top_left = {
    ((f32) tex.x * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_WIDTH, 
    ((f32) (tex.y+1) * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_HEIGHT
  };

  const Vec4F top_right = {
    ((f32) (tex.x+1) * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_WIDTH, 
    ((f32) (tex.y+1) * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_HEIGHT
  };

  const Vec4F bot_right = {
    ((f32) (tex.x+1) * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_WIDTH, 
    ((f32) tex.y * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_HEIGHT
  };

  const Vec4F bot_left = {
    ((f32) tex.x * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_WIDTH, 
    ((f32) tex.y * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_HEIGHT
  };

  r_push_vertex(renderer, v4f(p0.x, p0.y, p0.z, 0.0f), tint, top_left);
  r_push_vertex(renderer, v4f(p1.x, p1.y, p1.z, 0.0f), tint, top_right);
  r_push_vertex(renderer, v4f(p2.x, p2.y, p2.z, 0.0f), tint, bot_right);
  r_push_vertex(renderer, v4f(p3.x, p3.y, p3.z, 0.0f), tint, bot_left);
  r_push_quad_indices(renderer);
}

void draw_sprite_x(Mat3x3F xform, Vec4F tint, TextureID tex)
{
  R_Renderer *renderer = &GLOBAL->renderer;
  r_use_texture(renderer, &GLOBAL->resources.textures[D_TEXTURE_SPRITE]);
  r_use_shader(renderer, &GLOBAL->resources.shaders[D_SHADER_SPRITE]);

  Vec3F p0 = transform_3f(v3f(-8.0f,  8.0f, 1.0f), xform); // tl
  Vec3F p1 = transform_3f(v3f( 8.0f,  8.0f, 1.0f), xform); // tr
  Vec3F p2 = transform_3f(v3f( 8.0f, -8.0f, 1.0f), xform); // br
  Vec3F p3 = transform_3f(v3f(-8.0f, -8.0f, 1.0f), xform); // bl

  tex.y = D_SPRITE_SHEET_COUNT_Y - tex.y - 1;

  const Vec4F top_left = {
    ((f32) tex.x * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_WIDTH, 
    ((f32) (tex.y+1) * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_HEIGHT
  };

  const Vec4F top_right = {
    ((f32) (tex.x+1) * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_WIDTH, 
    ((f32) (tex.y+1) * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_HEIGHT
  };

  const Vec4F bot_right = {
    ((f32) (tex.x+1) * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_WIDTH, 
    ((f32) tex.y * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_HEIGHT
  };

  const Vec4F bot_left = {
    ((f32) tex.x * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_WIDTH, 
    ((f32) tex.y * D_SPRITE_SHEET_SIZE) / D_SPRITE_SHEET_HEIGHT
  };

  r_push_vertex(renderer, v4f(p0.x, p0.y, p0.z, 0.0f), tint, top_left);
  r_push_vertex(renderer, v4f(p1.x, p1.y, p1.z, 0.0f), tint, top_right);
  r_push_vertex(renderer, v4f(p2.x, p2.y, p2.z, 0.0f), tint, bot_right);
  r_push_vertex(renderer, v4f(p3.x, p3.y, p3.z, 0.0f), tint, bot_left);
  r_push_quad_indices(renderer);
}

void draw_particles(Entity *en)
{
  for (i32 i = 0; i < en->particle_desc.count; i++)
  {
    Particle *particle = &en->particles[i];
    f32 rot = particle->rot * RADIANS;
    draw_rectangle(particle->pos, particle->scale, rot, particle->color);
  }
}
