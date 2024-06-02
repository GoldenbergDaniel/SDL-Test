#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#elif defined(_WIN32)
#include "glad/glad.h"
#endif

#include "base/base_inc.h"
#include "render/render.h"
#include "render/shaders.h"
#include "ui/ui.h"
#include "game.h"
#include "entity.h"
#include "draw.h"

extern Globals global;

// @Glyphs ///////////////////////////////////////////////////////////////////////////////

UI_Glyph GLYPH_TABLE[127] = {
  ['A'] = ui_glyph(0, 0, 5, 8, 0, 0),
  ['B'] = ui_glyph(1, 0, 5, 8, 0, 0),
  ['C'] = ui_glyph(2, 0, 5, 8, 0, 0),
  ['D'] = ui_glyph(3, 0, 5, 8, 0, 0),
  ['E'] = ui_glyph(4, 0, 5, 8, 0, 0),
  ['F'] = ui_glyph(5, 0, 5, 8, 0, 0),
  ['G'] = ui_glyph(6, 0, 5, 8, 0, 0),
  ['H'] = ui_glyph(7, 0, 5, 8, 0, 0),
  ['I'] = ui_glyph(8, 0, 5, 8, 0, 0),
  ['J'] = ui_glyph(9, 0, 5, 8, 0, 0),
  ['K'] = ui_glyph(10, 0, 5, 8, 0, 0),
  ['L'] = ui_glyph(11, 0, 5, 8, 0, 0),
  ['M'] = ui_glyph(12, 0, 5, 8, 0, 0),
  ['N'] = ui_glyph(13, 0, 5, 8, 0, 0),
  ['O'] = ui_glyph(14, 0, 6, 8, 0, 0),
  ['P'] = ui_glyph(15, 0, 5, 8, 0, 0),
  ['Q'] = ui_glyph(0, 1, 6, 8, 0, 0),
  ['R'] = ui_glyph(1, 1, 5, 8, 0, 0),
  ['S'] = ui_glyph(2, 1, 5, 8, 0, 0),
  ['T'] = ui_glyph(3, 1, 5, 8, 0, 0),
  ['U'] = ui_glyph(4, 1, 5, 8, 0, 0),
  ['V'] = ui_glyph(5, 1, 5, 8, 0, 0),
  ['W'] = ui_glyph(6, 1, 5, 8, 0, 0),
  ['X'] = ui_glyph(7, 1, 5, 8, 0, 0),
  ['Y'] = ui_glyph(8, 1, 5, 8, 0, 0),
  ['Z'] = ui_glyph(9, 1, 5, 8, 0, 0),

  ['a'] = ui_glyph(0, 2, 4, 8, 0, 0),
  ['b'] = ui_glyph(1, 2, 4, 8, 0, 0),
  ['c'] = ui_glyph(2, 2, 4, 8, 0, 0),
  ['d'] = ui_glyph(3, 2, 4, 8, 0, 0),
  ['e'] = ui_glyph(4, 2, 4, 8, 0, 0),
  ['f'] = ui_glyph(5, 2, 4, 8, 0, 0),
  ['g'] = ui_glyph(6, 2, 4, 8, 0, -1),
  ['h'] = ui_glyph(7, 2, 4, 8, 0, 0),
  ['i'] = ui_glyph(8, 2, 1, 8, 0, 0),
  ['j'] = ui_glyph(9, 2, 3, 8, 0, -1),
  ['k'] = ui_glyph(10, 2, 4, 8, 0, 0),
  ['l'] = ui_glyph(11, 2, 2, 8, 0, 0),
  ['m'] = ui_glyph(12, 2, 5, 8, 0, 0),
  ['n'] = ui_glyph(13, 2, 4, 8, 0, 0),
  ['o'] = ui_glyph(14, 2, 5, 8, 0, 0),
  ['p'] = ui_glyph(15, 2, 4, 8, 0, -1),
  ['q'] = ui_glyph(0, 3, 5, 8, 0, -1),
  ['r'] = ui_glyph(1, 3, 4, 8, 0, 0),
  ['s'] = ui_glyph(2, 3, 4, 8, 0, 0),
  ['t'] = ui_glyph(3, 3, 3, 8, 0, 0),
  ['u'] = ui_glyph(4, 3, 5, 8, 0, 0),
  ['v'] = ui_glyph(5, 3, 5, 8, 0, 0),
  ['w'] = ui_glyph(6, 3, 5, 8, 0, 0),
  ['x'] = ui_glyph(7, 3, 5, 8, 0, 0),
  ['y'] = ui_glyph(8, 3, 5, 8, 0, -1),
  ['z'] = ui_glyph(9, 3, 5, 8, 0, 0),

  ['0'] = ui_glyph(0, 4, 5, 8, 0, 0),
  ['1'] = ui_glyph(1, 4, 5, 8, 0, 0),
  ['2'] = ui_glyph(2, 4, 5, 8, 0, 0),
  ['3'] = ui_glyph(3, 4, 5, 8, 0, 0),
  ['4'] = ui_glyph(4, 4, 5, 8, 0, 0),
  ['5'] = ui_glyph(5, 4, 5, 8, 0, 0),
  ['6'] = ui_glyph(6, 4, 5, 8, 0, 0),
  ['7'] = ui_glyph(7, 4, 5, 8, 0, 0),
  ['8'] = ui_glyph(8, 4, 5, 8, 0, 0),
  ['9'] = ui_glyph(9, 4, 5, 8, 0, 0),

  ['@'] = ui_glyph(0, 5, 6, 8, 0, 0),
  ['#'] = ui_glyph(1, 5, 5, 8, 0, 0),
  ['$'] = ui_glyph(2, 5, 4, 8, 0, 0),
  ['%'] = ui_glyph(3, 5, 5, 8, 0, 0),
  ['&'] = ui_glyph(4, 5, 5, 8, 0, 0),
  ['+'] = ui_glyph(5, 5, 5, 8, 0, 0),
  ['-'] = ui_glyph(6, 5, 5, 8, 0, 0),
  ['='] = ui_glyph(9, 5, 5, 8, 0, 0),
  ['['] = ui_glyph(10, 5, 2, 8, 0, 0),
  [']'] = ui_glyph(11, 5, 2, 8, 0, 0),
  ['{'] = ui_glyph(11, 5, 2, 8, 0, 0),
  ['}'] = ui_glyph(11, 5, 2, 8, 0, 0),
  ['('] = ui_glyph(12, 5, 3, 8, 0, 0),
  [')'] = ui_glyph(13, 5, 3, 8, 0, 0),
  ['<'] = ui_glyph(14, 5, 5, 8, 0, 0),
  ['>'] = ui_glyph(15, 5, 5, 8, 0, 0),
  ['!'] = ui_glyph(0, 6, 1, 8, 0, 0),
  ['?'] = ui_glyph(1, 6, 3, 8, 0, 0),
  [':'] = ui_glyph(2, 6, 1, 8, 0, 0),
  [';'] = ui_glyph(3, 6, 2, 8, 0, 0),
  ['/'] = ui_glyph(4, 6, 3, 8, 0, 0),
  ['\\'] = ui_glyph(5, 6, 3, 8, 0, 0),
  ['.'] = ui_glyph(6, 6, 1, 8, 0, 0),
  [','] = ui_glyph(7, 6, 2, 8, 0, 0),
  ['_'] = ui_glyph(8, 6, 5, 8, 0, 0),
  ['|'] = ui_glyph(9, 6, 1, 8, 0, 0),
  ['\''] = ui_glyph(10, 6, 1, 8, 0, 0),
  ['\"'] = ui_glyph(11, 6, 3, 8, 0, 0),
  ['~'] = ui_glyph(12, 6, 6, 8, 0, 0),
  ['~'] = ui_glyph(13, 6, 6, 8, 0, 0),
  ['^'] = ui_glyph(14, 6, 3, 8, 0, 0),
  ['*'] = ui_glyph(15, 6, 3, 8, 0, 0),
};

inline
UI_Glyph get_glyph(char glyph)
{
  return GLYPH_TABLE[glyph];
}

// @Assets ///////////////////////////////////////////////////////////////////////////////

Resources load_resources(Arena *arena, String path)
{
  Resources res = {0};
  res.textures = arena_push(arena, sizeof (R_Texture) * TEXTURE_COUNT);
  res.shaders = arena_push(arena, sizeof (R_Shader) * SHADER_COUNT);

  R_Shader primitive_shader = r_create_shader(PRIMITIVE_VERT_SRC, PRIMITIVE_FRAG_SRC);
  res.shaders[0] = primitive_shader;

  R_Shader sprite_shader = r_create_shader(SPRITE_VERT_SRC, SPRITE_FRAG_SRC);
  res.shaders[1] = sprite_shader;

  Arena scratch = get_scratch_arena(arena);
  {
    String path_to_texture;
    R_Texture texture;

    path_to_texture = str_concat(path, str("/texture/sprites.png"), &scratch);
    texture = r_create_texture(path_to_texture);
    res.textures[0] = texture;

    path_to_texture = str_concat(path, str("/texture/font.png"), &scratch);
    texture = r_create_texture(path_to_texture);
    res.textures[1] = texture;
    
    path_to_texture = str_concat(path, str("/texture/scene.png"), &scratch);
    texture = r_create_texture(path_to_texture);
    res.textures[2] = texture;
  }
  arena_clear(&scratch);

  return res;
}

R_Shader *get_shader(u8 type)
{
  return &global.resources.shaders[type];
}

// @Draw /////////////////////////////////////////////////////////////////////////////////

static inline
Vec2F get_tl(Vec2I coords, f32 size, f32 w, f32 h)
{
  return (Vec2F) {
    ((f32) coords.x * size) / w, 
    ((f32) (coords.y+1) * size) / h
  };
}

static inline
Vec2F get_tr(Vec2I coords, f32 size, f32 w, f32 h)
{
  return (Vec2F) {
    ((f32) (coords.x+1) * size) / w, 
    ((f32) (coords.y+1) * size) / h
  };
}

static inline
Vec2F get_br(Vec2I coords, f32 size, f32 w, f32 h)
{
  return (Vec2F) {
    ((f32) (coords.x+1) * size) / w, 
    ((f32) coords.y * size) / h
  };
}

static inline
Vec2F get_bl(Vec2I coords, f32 size, f32 w, f32 h)
{
  return (Vec2F) {
    ((f32) coords.x * size) / w, 
    ((f32) coords.y * size) / h
  };
}

inline
void clear_frame(Vec4F color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void draw_rectangle(Vec2F pos, Vec2F dim, f32 rot, Vec4F tint)
{
  R_Renderer *renderer = &global.renderer;
  r_use_shader(renderer, &global.resources.shaders[SHADER_PRIMITIVE]);

  Mat3x3F xform = m3x3f(1.0f);
  xform = mul_3x3f(rotate_3x3f(rot), xform);
  xform = mul_3x3f(translate_3x3f(pos.x, pos.y), xform);
  
  Vec3F p0 = transform_3f(v3f(0.0f, dim.height, 1.0f), xform);
  Vec3F p1 = transform_3f(v3f(dim.width, dim.height, 1.0f), xform);
  Vec3F p2 = transform_3f(v3f(dim.width, 0.0f, 1.0f), xform);
  Vec3F p3 = transform_3f(v3f(0.0f, 0.0f, 1.0f), xform);

  r_push_vertex(renderer, p0, tint, V4F_ZERO, V2F_ZERO);
  r_push_vertex(renderer, p1, tint, V4F_ZERO, V2F_ZERO);
  r_push_vertex(renderer, p2, tint, V4F_ZERO, V2F_ZERO);
  r_push_vertex(renderer, p3, tint, V4F_ZERO, V2F_ZERO);
  r_push_quad_indices(renderer);
}

void draw_rectangle_v(Vec3F p0, Vec3F p1, Vec3F p2, Vec3F p3, Vec4F tint)
{
  R_Renderer *renderer = &global.renderer;
  r_use_shader(renderer, &global.resources.shaders[SHADER_PRIMITIVE]);

  r_push_vertex(renderer, p0, tint, V4F_ZERO, V2F_ZERO);
  r_push_vertex(renderer, p1, tint, V4F_ZERO, V2F_ZERO);
  r_push_vertex(renderer, p2, tint, V4F_ZERO, V2F_ZERO);
  r_push_vertex(renderer, p3, tint, V4F_ZERO, V2F_ZERO);
  r_push_quad_indices(renderer);
}

void draw_rectangle_x(Mat3x3F xform, Vec4F tint)
{
  R_Renderer *renderer = &global.renderer;
  r_use_shader(renderer, &global.resources.shaders[SHADER_PRIMITIVE]);

  Vec3F p0 = transform_3f(v3f(-8.0f,  8.0f, 1.0f), xform); // tl
  Vec3F p1 = transform_3f(v3f( 8.0f,  8.0f, 1.0f), xform); // tr
  Vec3F p2 = transform_3f(v3f( 8.0f, -8.0f, 1.0f), xform); // br
  Vec3F p3 = transform_3f(v3f(-8.0f, -8.0f, 1.0f), xform); // bl

  r_push_vertex(renderer, p0, tint, V4F_ZERO, V2F_ZERO);
  r_push_vertex(renderer, p1, tint, V4F_ZERO, V2F_ZERO);
  r_push_vertex(renderer, p2, tint, V4F_ZERO, V2F_ZERO);
  r_push_vertex(renderer, p3, tint, V4F_ZERO, V2F_ZERO);
  r_push_quad_indices(renderer);
}

void draw_sprite(Vec2F pos, Vec2F dim, f32 rot, Vec4F tint, TextureID tex, bool flash)
{
  R_Renderer *renderer = &global.renderer;
  r_use_texture(renderer, &global.resources.textures[TEXTURE_SPRITE]);
  r_use_shader(renderer, &global.resources.shaders[SHADER_SPRITE]);

  Mat3x3F xform = m3x3f(1.0f);
  xform = mul_3x3f(scale_3x3f(dim.x, dim.y), xform);
  xform = mul_3x3f(rotate_3x3f(rot * RADIANS), xform);
  xform = mul_3x3f(translate_3x3f(pos.x, pos.y), xform);
  
  Vec3F p0 = transform_3f(v3f(0.0f, 1.0f, 1.0f), xform); // tl
  Vec3F p1 = transform_3f(v3f(1.0f, 1.0f, 1.0f), xform); // tr
  Vec3F p2 = transform_3f(v3f(1.0f, 0.0f, 1.0f), xform); // br
  Vec3F p3 = transform_3f(v3f(0.0f, 0.0f, 1.0f), xform); // bl

  tex.y = (SPRITE_ATLAS_HEIGHT/SPRITE_ATLAS_CELL) - tex.y - 1;
  Vec2F top_left = get_tl(tex, SPRITE_ATLAS_CELL, SPRITE_ATLAS_WIDTH, SPRITE_ATLAS_HEIGHT);
  Vec2F top_right = get_tr(tex, SPRITE_ATLAS_CELL, SPRITE_ATLAS_WIDTH, SPRITE_ATLAS_HEIGHT);
  Vec2F bot_right = get_br(tex, SPRITE_ATLAS_CELL, SPRITE_ATLAS_WIDTH, SPRITE_ATLAS_HEIGHT);
  Vec2F bot_left = get_bl(tex, SPRITE_ATLAS_CELL, SPRITE_ATLAS_WIDTH, SPRITE_ATLAS_HEIGHT);

  Vec4F color = flash ? v4f(1, 1, 1, 0) : v4f(0, 0, 0, 0);

  r_push_vertex(renderer, p0, tint, color, top_left);
  r_push_vertex(renderer, p1, tint, color, top_right);
  r_push_vertex(renderer, p2, tint, color, bot_right);
  r_push_vertex(renderer, p3, tint, color, bot_left);
  r_push_quad_indices(renderer);
}

void draw_sprite_v(Vec3F p0, Vec3F p1, Vec3F p2, Vec3F p3, Vec4F tint, TextureID tex, bool flash)
{
  R_Renderer *renderer = &global.renderer;
  r_use_texture(renderer, &global.resources.textures[TEXTURE_SPRITE]);
  r_use_shader(renderer, &global.resources.shaders[SHADER_SPRITE]);

  tex.y = (SPRITE_ATLAS_HEIGHT/SPRITE_ATLAS_CELL) - tex.y - 1;
  Vec2F top_left = get_tl(tex, SPRITE_ATLAS_CELL, SPRITE_ATLAS_WIDTH, SPRITE_ATLAS_HEIGHT);
  Vec2F top_right = get_tr(tex, SPRITE_ATLAS_CELL, SPRITE_ATLAS_WIDTH, SPRITE_ATLAS_HEIGHT);
  Vec2F bot_right = get_br(tex, SPRITE_ATLAS_CELL, SPRITE_ATLAS_WIDTH, SPRITE_ATLAS_HEIGHT);
  Vec2F bot_left = get_bl(tex, SPRITE_ATLAS_CELL, SPRITE_ATLAS_WIDTH, SPRITE_ATLAS_HEIGHT);

  Vec4F color = flash ? v4f(1, 1, 1, 0) : v4f(0, 0, 0, 0);

  r_push_vertex(renderer, p0, tint, color, top_left);
  r_push_vertex(renderer, p1, tint, color, top_right);
  r_push_vertex(renderer, p2, tint, color, bot_right);
  r_push_vertex(renderer, p3, tint, color, bot_left);
  r_push_quad_indices(renderer);
}

void draw_sprite_x(Mat3x3F xform, Vec4F tint, TextureID tex, bool flash)
{
  R_Renderer *renderer = &global.renderer;
  r_use_texture(renderer, &global.resources.textures[TEXTURE_SPRITE]);
  r_use_shader(renderer, &global.resources.shaders[SHADER_SPRITE]);

  Vec3F p0 = transform_3f(v3f(-8.0f,  8.0f, 1.0f), xform); // tl
  Vec3F p1 = transform_3f(v3f( 8.0f,  8.0f, 1.0f), xform); // tr
  Vec3F p2 = transform_3f(v3f( 8.0f, -8.0f, 1.0f), xform); // br
  Vec3F p3 = transform_3f(v3f(-8.0f, -8.0f, 1.0f), xform); // bl

  tex.y = (SPRITE_ATLAS_HEIGHT/SPRITE_ATLAS_CELL) - tex.y - 1;
  Vec2F top_left = get_tl(tex, SPRITE_ATLAS_CELL, SPRITE_ATLAS_WIDTH, SPRITE_ATLAS_HEIGHT);
  Vec2F top_right = get_tr(tex, SPRITE_ATLAS_CELL, SPRITE_ATLAS_WIDTH, SPRITE_ATLAS_HEIGHT);
  Vec2F bot_right = get_br(tex, SPRITE_ATLAS_CELL, SPRITE_ATLAS_WIDTH, SPRITE_ATLAS_HEIGHT);
  Vec2F bot_left = get_bl(tex, SPRITE_ATLAS_CELL, SPRITE_ATLAS_WIDTH, SPRITE_ATLAS_HEIGHT);

  Vec4F color = flash ? v4f(1, 1, 1, 0) : v4f(0, 0, 0, 0);

  r_push_vertex(renderer, p0, tint, color, top_left);
  r_push_vertex(renderer, p1, tint, color, top_right);
  r_push_vertex(renderer, p2, tint, color, bot_right);
  r_push_vertex(renderer, p3, tint, color, bot_left);
  r_push_quad_indices(renderer);
}

static inline

Vec2F uv_tl(Vec2I coords, f32 size, f32 w, f32 h)
{
  return (Vec2F) {
    (((coords.x  ) * size)) / w, 
    (((coords.y+1) * size)) / h
  };
}

static inline
Vec2F uv_tr(Vec2I coords, f32 size, f32 w, f32 h)
{
  return (Vec2F) {
    (((coords.x+1) * size)) / w, 
    (((coords.y+1) * size)) / h
  };
}

static inline

Vec2F uv_br(Vec2I coords, f32 size, f32 w, f32 h)
{
  return (Vec2F) {
    (((coords.x+1) * size)) / w, 
    (((coords.y  ) * size)) / h
  };
}

static inline
Vec2F uv_bl(Vec2I coords, f32 size, f32 w, f32 h)
{
  return (Vec2F) {
    (((coords.x  ) * size)) / w, 
    (((coords.y  ) * size)) / h
  };
}

void draw_glyph(Vec2F pos, f32 size, Vec4F tint, TextureID tex)
{
  R_Renderer *renderer = &global.renderer;
  r_use_texture(renderer, &global.resources.textures[TEXTURE_FONT]);
  r_use_shader(renderer, &global.resources.shaders[SHADER_SPRITE]);

  Mat3x3F xform = m3x3f(1.0f);
  xform = mul_3x3f(scale_3x3f(size, size), xform);
  xform = mul_3x3f(translate_3x3f(pos.x, pos.y), xform);
  
  Vec3F p0 = transform_3f(v3f(0.0f, 1.0f, 1.0f), xform); // tl
  Vec3F p1 = transform_3f(v3f(1.0f, 1.0f, 1.0f), xform); // tr
  Vec3F p2 = transform_3f(v3f(1.0f, 0.0f, 1.0f), xform); // br
  Vec3F p3 = transform_3f(v3f(0.0f, 0.0f, 1.0f), xform); // bl

  tex.y = (GLYPH_ATLAS_HEIGHT/GLYPH_ATLAS_CELL) - tex.y - 1;
  Vec2F top_left = uv_tl(tex, GLYPH_ATLAS_CELL, GLYPH_ATLAS_WIDTH, GLYPH_ATLAS_HEIGHT);
  Vec2F top_right = uv_tr(tex, GLYPH_ATLAS_CELL, GLYPH_ATLAS_WIDTH, GLYPH_ATLAS_HEIGHT);
  Vec2F bot_right = uv_br(tex, GLYPH_ATLAS_CELL, GLYPH_ATLAS_WIDTH, GLYPH_ATLAS_HEIGHT);
  Vec2F bot_left = uv_bl(tex, GLYPH_ATLAS_CELL, GLYPH_ATLAS_WIDTH, GLYPH_ATLAS_HEIGHT);

  r_push_vertex(renderer, p0, tint, V4F_ZERO, top_left);
  r_push_vertex(renderer, p1, tint, V4F_ZERO, top_right);
  r_push_vertex(renderer, p2, tint, V4F_ZERO, bot_right);
  r_push_vertex(renderer, p3, tint, V4F_ZERO, bot_left);
  r_push_quad_indices(renderer);
}

void draw_scene(Vec2F pos, Vec2F dim, Vec4F tint)
{
  R_Renderer *renderer = &global.renderer;
  r_use_texture(renderer, &global.resources.textures[TEXTURE_SCENE]);
  r_use_shader(renderer, &global.resources.shaders[SHADER_SPRITE]);

  Mat3x3F xform = m3x3f(1.0f);
  xform = mul_3x3f(translate_3x3f(pos.x, pos.y), xform);
  
  Vec3F p0 = transform_3f(v3f(0.0f, dim.y, 1.0f), xform); // tl
  Vec3F p1 = transform_3f(v3f(dim.x, dim.y, 1.0f), xform); // tr
  Vec3F p2 = transform_3f(v3f(dim.x, 0.0f, 1.0f), xform); // br
  Vec3F p3 = transform_3f(v3f(0.0f, 0.0f, 1.0f), xform); // bl

  r_push_vertex(renderer, p0, tint, V4F_ZERO, v2f(0, 1));
  r_push_vertex(renderer, p1, tint, V4F_ZERO, v2f(1, 1));
  r_push_vertex(renderer, p2, tint, V4F_ZERO, v2f(1, 0));
  r_push_vertex(renderer, p3, tint, V4F_ZERO, v2f(0, 0));
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
