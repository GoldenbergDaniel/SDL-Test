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
#include "globals.h"
#include "entity.h"
#include "draw.h"

extern Globals *GLOBAL;

// @Glyphs ///////////////////////////////////////////////////////////////////////////////

UI_Glyph GLYPH_TABLE[127] = {
  ['A'] = ui_glyph(0, 0, 8, 8),
  ['B'] = ui_glyph(1, 0, 8, 8),
  ['C'] = ui_glyph(2, 0, 8, 8),
  ['D'] = ui_glyph(3, 0, 8, 8),
  ['E'] = ui_glyph(4, 0, 8, 8),
  ['F'] = ui_glyph(5, 0, 8, 8),
  ['G'] = ui_glyph(6, 0, 8, 8),
  ['H'] = ui_glyph(7, 0, 8, 8),
  ['I'] = ui_glyph(8, 0, 8, 8),
  ['J'] = ui_glyph(9, 0, 8, 8),
  ['K'] = ui_glyph(10, 0, 8, 8),
  ['L'] = ui_glyph(11, 0, 8, 8),
  ['M'] = ui_glyph(12, 0, 8, 8),
  ['N'] = ui_glyph(13, 0, 8, 8),
  ['O'] = ui_glyph(14, 0, 8, 8),
  ['P'] = ui_glyph(15, 0, 8, 8),
  ['Q'] = ui_glyph(0, 1, 8, 8),
  ['R'] = ui_glyph(1, 1, 8, 8),
  ['S'] = ui_glyph(2, 1, 8, 8),
  ['T'] = ui_glyph(3, 1, 8, 8),
  ['U'] = ui_glyph(4, 1, 8, 8),
  ['V'] = ui_glyph(5, 1, 8, 8),
  ['W'] = ui_glyph(0, 1, 8, 8),
  ['X'] = ui_glyph(0, 1, 8, 8),
  ['Y'] = ui_glyph(0, 1, 8, 8),
  ['Z'] = ui_glyph(0, 1, 8, 8),

  ['a'] = ui_glyph(0, 2, 8, 8),
  ['b'] = ui_glyph(1, 2, 8, 8),
  ['c'] = ui_glyph(2, 2, 8, 8),
  ['d'] = ui_glyph(3, 2, 8, 8),
  ['e'] = ui_glyph(4, 2, 8, 8),
  ['f'] = ui_glyph(5, 2, 8, 8),
  ['g'] = ui_glyph(6, 2, 8, 8),
  ['h'] = ui_glyph(7, 2, 8, 8),
  ['i'] = ui_glyph(8, 2, 8, 8),
  ['j'] = ui_glyph(9, 2, 8, 8),
  ['k'] = ui_glyph(10, 2, 8, 8),
  ['l'] = ui_glyph(11, 2, 8, 8),
  ['m'] = ui_glyph(12, 2, 8, 8),
  ['n'] = ui_glyph(13, 2, 8, 8),
  ['o'] = ui_glyph(14, 2, 8, 8),
  ['p'] = ui_glyph(15, 2, 8, 8),
  ['q'] = ui_glyph(0, 3, 8, 8),
  ['r'] = ui_glyph(1, 3, 8, 8),
  ['s'] = ui_glyph(2, 3, 8, 8),
  ['t'] = ui_glyph(3, 3, 8, 8),
  ['u'] = ui_glyph(4, 3, 8, 8),
  ['v'] = ui_glyph(5, 3, 8, 8),
  ['w'] = ui_glyph(6, 3, 8, 8),
  ['x'] = ui_glyph(7, 3, 8, 8),
  ['y'] = ui_glyph(8, 3, 8, 8),
  ['z'] = ui_glyph(9, 3, 8, 8),

  ['0'] = ui_glyph(0, 4, 8, 8),
  ['1'] = ui_glyph(1, 4, 8, 8),
  ['2'] = ui_glyph(2, 4, 8, 8),
  ['3'] = ui_glyph(3, 4, 8, 8),
  ['4'] = ui_glyph(4, 4, 8, 8),
  ['5'] = ui_glyph(5, 4, 8, 8),
  ['6'] = ui_glyph(6, 4, 8, 8),
  ['7'] = ui_glyph(7, 4, 8, 8),
  ['8'] = ui_glyph(8, 4, 8, 8),
  ['9'] = ui_glyph(9, 4, 8, 8),

  ['@'] = ui_glyph(0, 5, 8, 8),
  ['#'] = ui_glyph(1, 5, 8, 8),
  ['$'] = ui_glyph(2, 5, 8, 8),
  ['%'] = ui_glyph(3, 5, 8, 8),
  ['&'] = ui_glyph(4, 5, 8, 8),
  ['^'] = ui_glyph(5, 5, 8, 8),
  ['*'] = ui_glyph(6, 5, 8, 8),
  ['+'] = ui_glyph(7, 5, 8, 8),
  ['-'] = ui_glyph(8, 5, 8, 8),
  ['='] = ui_glyph(9, 5, 8, 8),
  ['['] = ui_glyph(10, 5, 8, 8),
  [']'] = ui_glyph(11, 5, 8, 8),
  ['('] = ui_glyph(12, 5, 8, 8),
  [')'] = ui_glyph(13, 5, 8, 8),
  ['<'] = ui_glyph(14, 5, 8, 8),
  ['>'] = ui_glyph(15, 5, 8, 8),
  ['!'] = ui_glyph(0, 6, 8, 8),
  ['?'] = ui_glyph(1, 6, 8, 8),
  [':'] = ui_glyph(2, 6, 8, 8),
  [';'] = ui_glyph(3, 6, 8, 8),
  ['/'] = ui_glyph(4, 6, 8, 8),
  ['\\'] = ui_glyph(5, 6, 8, 8),
  ['.'] = ui_glyph(6, 6, 8, 8),
  [','] = ui_glyph(7, 6, 8, 8),
  ['_'] = ui_glyph(8, 6, 8, 8),
  ['~'] = ui_glyph(9, 6, 8, 8),
  ['|'] = ui_glyph(10, 6, 8, 8),
  ['\''] = ui_glyph(11, 6, 8, 8),
  ['\"'] = ui_glyph(12, 6, 8, 8),
  [' '] = ui_glyph(13, 6, 8, 8),
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

  Arena scratch = arena_get_scratch(arena);
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
  return &GLOBAL->resources.shaders[type];
}

// @Draw /////////////////////////////////////////////////////////////////////////////////

static inline
Vec4F get_tl(Vec2I coords, f32 size, f32 w, f32 h)
{
  return (Vec4F) {
    ((f32) coords.x * size) / w, 
    ((f32) (coords.y+1) * size) / h,
    0, 0,
  };
}

static inline
Vec4F get_tr(Vec2I coords, f32 size, f32 w, f32 h)
{
  return (Vec4F) {
    ((f32) (coords.x+1) * size) / w, 
    ((f32) (coords.y+1) * size) / h,
    0, 0,
  };
}

static inline
Vec4F get_br(Vec2I coords, f32 size, f32 w, f32 h)
{
  return (Vec4F) {
    ((f32) (coords.x+1) * size) / w, 
    ((f32) coords.y * size) / h,
    0, 0,
  };
}

static inline
Vec4F get_bl(Vec2I coords, f32 size, f32 w, f32 h)
{
  return (Vec4F) {
    ((f32) coords.x * size) / w, 
    ((f32) coords.y * size) / h,
    0, 0,
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
  R_Renderer *renderer = &GLOBAL->renderer;
  r_use_shader(renderer, &GLOBAL->resources.shaders[SHADER_PRIMITIVE]);

  Mat3x3F xform = m3x3f(1.0f);
  xform = mul_3x3f(rotate_3x3f(rot), xform);
  xform = mul_3x3f(translate_3x3f(pos.x, pos.y), xform);
  
  Vec3F p0 = transform_3f(v3f(0.0f, dim.height, 1.0f), xform);
  Vec3F p1 = transform_3f(v3f(dim.width, dim.height, 1.0f), xform);
  Vec3F p2 = transform_3f(v3f(dim.width, 0.0f, 1.0f), xform);
  Vec3F p3 = transform_3f(v3f(0.0f, 0.0f, 1.0f), xform);

  r_push_vertex(renderer, v4f(p0.x, p0.y, p0.z, 0.0f), tint, V4F_ZERO, V4F_ZERO);
  r_push_vertex(renderer, v4f(p1.x, p1.y, p1.z, 0.0f), tint, V4F_ZERO, V4F_ZERO);
  r_push_vertex(renderer, v4f(p2.x, p2.y, p2.z, 0.0f), tint, V4F_ZERO, V4F_ZERO);
  r_push_vertex(renderer, v4f(p3.x, p3.y, p3.z, 0.0f), tint, V4F_ZERO, V4F_ZERO);
  r_push_quad_indices(renderer);
}

void draw_rectangle_v(Vec3F p0, Vec3F p1, Vec3F p2, Vec3F p3, Vec4F tint)
{
  R_Renderer *renderer = &GLOBAL->renderer;
  r_use_shader(renderer, &GLOBAL->resources.shaders[SHADER_PRIMITIVE]);

  r_push_vertex(renderer, v4f(p0.x, p0.y, p0.z, 0.0f), tint, V4F_ZERO, V4F_ZERO);
  r_push_vertex(renderer, v4f(p1.x, p1.y, p1.z, 0.0f), tint, V4F_ZERO, V4F_ZERO);
  r_push_vertex(renderer, v4f(p2.x, p2.y, p2.z, 0.0f), tint, V4F_ZERO, V4F_ZERO);
  r_push_vertex(renderer, v4f(p3.x, p3.y, p3.z, 0.0f), tint, V4F_ZERO, V4F_ZERO);
  r_push_quad_indices(renderer);
}

void draw_rectangle_x(Mat3x3F xform, Vec4F tint)
{
  R_Renderer *renderer = &GLOBAL->renderer;
  r_use_shader(renderer, &GLOBAL->resources.shaders[SHADER_PRIMITIVE]);

  Vec3F p0 = transform_3f(v3f(-8.0f,  8.0f, 1.0f), xform); // tl
  Vec3F p1 = transform_3f(v3f( 8.0f,  8.0f, 1.0f), xform); // tr
  Vec3F p2 = transform_3f(v3f( 8.0f, -8.0f, 1.0f), xform); // br
  Vec3F p3 = transform_3f(v3f(-8.0f, -8.0f, 1.0f), xform); // bl

  r_push_vertex(renderer, v4f(p0.x, p0.y, p0.z, 0.0f), tint, V4F_ZERO, V4F_ZERO);
  r_push_vertex(renderer, v4f(p1.x, p1.y, p1.z, 0.0f), tint, V4F_ZERO, V4F_ZERO);
  r_push_vertex(renderer, v4f(p2.x, p2.y, p2.z, 0.0f), tint, V4F_ZERO, V4F_ZERO);
  r_push_vertex(renderer, v4f(p3.x, p3.y, p3.z, 0.0f), tint, V4F_ZERO, V4F_ZERO);
  r_push_quad_indices(renderer);
}

void draw_sprite(Vec2F pos, Vec2F dim, f32 rot, Vec4F tint, TextureID tex, bool flash)
{
  R_Renderer *renderer = &GLOBAL->renderer;
  r_use_texture(renderer, &GLOBAL->resources.textures[TEXTURE_SPRITE]);
  r_use_shader(renderer, &GLOBAL->resources.shaders[SHADER_SPRITE]);

  Mat3x3F xform = m3x3f(1.0f);
  xform = mul_3x3f(scale_3x3f(dim.x, dim.y), xform);
  xform = mul_3x3f(rotate_3x3f(rot * RADIANS), xform);
  xform = mul_3x3f(translate_3x3f(pos.x, pos.y), xform);
  
  Vec3F p0 = transform_3f(v3f(0.0f, 1.0f, 1.0f), xform); // tl
  Vec3F p1 = transform_3f(v3f(1.0f, 1.0f, 1.0f), xform); // tr
  Vec3F p2 = transform_3f(v3f(1.0f, 0.0f, 1.0f), xform); // br
  Vec3F p3 = transform_3f(v3f(0.0f, 0.0f, 1.0f), xform); // bl

  tex.y = (SPRITE_SHEET_HEIGHT/SPRITE_SHEET_SIZE) - tex.y - 1;
  Vec4F top_left = get_tl(tex, SPRITE_SHEET_SIZE, SPRITE_SHEET_WIDTH, SPRITE_SHEET_HEIGHT);
  Vec4F top_right = get_tr(tex, SPRITE_SHEET_SIZE, SPRITE_SHEET_WIDTH, SPRITE_SHEET_HEIGHT);
  Vec4F bot_right = get_br(tex, SPRITE_SHEET_SIZE, SPRITE_SHEET_WIDTH, SPRITE_SHEET_HEIGHT);
  Vec4F bot_left = get_bl(tex, SPRITE_SHEET_SIZE, SPRITE_SHEET_WIDTH, SPRITE_SHEET_HEIGHT);

  Vec4F color = flash ? v4f(1, 1, 1, 0) : v4f(0, 0, 0, 0);

  r_push_vertex(renderer, v4f(p0.x, p0.y, p0.z, 0.0f), tint, color, top_left);
  r_push_vertex(renderer, v4f(p1.x, p1.y, p1.z, 0.0f), tint, color, top_right);
  r_push_vertex(renderer, v4f(p2.x, p2.y, p2.z, 0.0f), tint, color, bot_right);
  r_push_vertex(renderer, v4f(p3.x, p3.y, p3.z, 0.0f), tint, color, bot_left);
  r_push_quad_indices(renderer);
}

void draw_sprite_v(Vec3F p0, Vec3F p1, Vec3F p2, Vec3F p3, Vec4F tint, TextureID tex, bool flash)
{
  R_Renderer *renderer = &GLOBAL->renderer;
  r_use_texture(renderer, &GLOBAL->resources.textures[TEXTURE_SPRITE]);
  r_use_shader(renderer, &GLOBAL->resources.shaders[SHADER_SPRITE]);

  tex.y = (SPRITE_SHEET_HEIGHT/SPRITE_SHEET_SIZE) - tex.y - 1;
  Vec4F top_left = get_tl(tex, SPRITE_SHEET_SIZE, SPRITE_SHEET_WIDTH, SPRITE_SHEET_HEIGHT);
  Vec4F top_right = get_tr(tex, SPRITE_SHEET_SIZE, SPRITE_SHEET_WIDTH, SPRITE_SHEET_HEIGHT);
  Vec4F bot_right = get_br(tex, SPRITE_SHEET_SIZE, SPRITE_SHEET_WIDTH, SPRITE_SHEET_HEIGHT);
  Vec4F bot_left = get_bl(tex, SPRITE_SHEET_SIZE, SPRITE_SHEET_WIDTH, SPRITE_SHEET_HEIGHT);

  Vec4F color = flash ? v4f(1, 1, 1, 0) : v4f(0, 0, 0, 0);

  r_push_vertex(renderer, v4f(p0.x, p0.y, p0.z, 0.0f), tint, color, top_left);
  r_push_vertex(renderer, v4f(p1.x, p1.y, p1.z, 0.0f), tint, color, top_right);
  r_push_vertex(renderer, v4f(p2.x, p2.y, p2.z, 0.0f), tint, color, bot_right);
  r_push_vertex(renderer, v4f(p3.x, p3.y, p3.z, 0.0f), tint, color, bot_left);
  r_push_quad_indices(renderer);
}

void draw_sprite_x(Mat3x3F xform, Vec4F tint, TextureID tex, bool flash)
{
  R_Renderer *renderer = &GLOBAL->renderer;
  r_use_texture(renderer, &GLOBAL->resources.textures[TEXTURE_SPRITE]);
  r_use_shader(renderer, &GLOBAL->resources.shaders[SHADER_SPRITE]);

  Vec3F p0 = transform_3f(v3f(-8.0f,  8.0f, 1.0f), xform); // tl
  Vec3F p1 = transform_3f(v3f( 8.0f,  8.0f, 1.0f), xform); // tr
  Vec3F p2 = transform_3f(v3f( 8.0f, -8.0f, 1.0f), xform); // br
  Vec3F p3 = transform_3f(v3f(-8.0f, -8.0f, 1.0f), xform); // bl

  tex.y = (SPRITE_SHEET_HEIGHT/SPRITE_SHEET_SIZE) - tex.y - 1;
  Vec4F top_left = get_tl(tex, SPRITE_SHEET_SIZE, SPRITE_SHEET_WIDTH, SPRITE_SHEET_HEIGHT);
  Vec4F top_right = get_tr(tex, SPRITE_SHEET_SIZE, SPRITE_SHEET_WIDTH, SPRITE_SHEET_HEIGHT);
  Vec4F bot_right = get_br(tex, SPRITE_SHEET_SIZE, SPRITE_SHEET_WIDTH, SPRITE_SHEET_HEIGHT);
  Vec4F bot_left = get_bl(tex, SPRITE_SHEET_SIZE, SPRITE_SHEET_WIDTH, SPRITE_SHEET_HEIGHT);

  Vec4F color = flash ? v4f(1, 1, 1, 0) : v4f(0, 0, 0, 0);

  r_push_vertex(renderer, v4f(p0.x, p0.y, p0.z, 0.0f), tint, color, top_left);
  r_push_vertex(renderer, v4f(p1.x, p1.y, p1.z, 0.0f), tint, color, top_right);
  r_push_vertex(renderer, v4f(p2.x, p2.y, p2.z, 0.0f), tint, color, bot_right);
  r_push_vertex(renderer, v4f(p3.x, p3.y, p3.z, 0.0f), tint, color, bot_left);
  r_push_quad_indices(renderer);
}

void draw_glyph(Vec2F pos, Vec2F dim, Vec4F tint, TextureID tex)
{
  R_Renderer *renderer = &GLOBAL->renderer;
  r_use_texture(renderer, &GLOBAL->resources.textures[TEXTURE_FONT]);
  r_use_shader(renderer, &GLOBAL->resources.shaders[SHADER_SPRITE]);

  Mat3x3F xform = m3x3f(1.0f);
  xform = mul_3x3f(translate_3x3f(pos.x, pos.y), xform);
  
  Vec3F p0 = transform_3f(v3f(0.0f, dim.y, 1.0f), xform); // tl
  Vec3F p1 = transform_3f(v3f(dim.x, dim.y, 1.0f), xform); // tr
  Vec3F p2 = transform_3f(v3f(dim.x, 0.0f, 1.0f), xform); // br
  Vec3F p3 = transform_3f(v3f(0.0f, 0.0f, 1.0f), xform); // bl

  tex.y = (FONT_SHEET_HEIGHT/FONT_SHEET_SIZE) - tex.y - 1;
  Vec4F top_left = get_tl(tex, FONT_SHEET_SIZE, FONT_SHEET_WIDTH, FONT_SHEET_HEIGHT);
  Vec4F top_right = get_tr(tex, FONT_SHEET_SIZE, FONT_SHEET_WIDTH, FONT_SHEET_HEIGHT);
  Vec4F bot_right = get_br(tex, FONT_SHEET_SIZE, FONT_SHEET_WIDTH, FONT_SHEET_HEIGHT);
  Vec4F bot_left = get_bl(tex, FONT_SHEET_SIZE, FONT_SHEET_WIDTH, FONT_SHEET_HEIGHT);

  r_push_vertex(renderer, v4f(p0.x, p0.y, p0.z, 0.0f), tint, V4F_ZERO, top_left);
  r_push_vertex(renderer, v4f(p1.x, p1.y, p1.z, 0.0f), tint, V4F_ZERO, top_right);
  r_push_vertex(renderer, v4f(p2.x, p2.y, p2.z, 0.0f), tint, V4F_ZERO, bot_right);
  r_push_vertex(renderer, v4f(p3.x, p3.y, p3.z, 0.0f), tint, V4F_ZERO, bot_left);
  r_push_quad_indices(renderer);
}

void draw_scene(Vec2F pos, Vec2F dim, Vec4F tint)
{
  R_Renderer *renderer = &GLOBAL->renderer;
  r_use_texture(renderer, &GLOBAL->resources.textures[TEXTURE_SCENE]);
  r_use_shader(renderer, &GLOBAL->resources.shaders[SHADER_SPRITE]);

  Mat3x3F xform = m3x3f(1.0f);
  xform = mul_3x3f(scale_3x3f(dim.x, dim.y), xform);
  xform = mul_3x3f(translate_3x3f(pos.x, pos.y), xform);
  
  Vec3F p0 = transform_3f(v3f(0.0f, 1.0f, 1.0f), xform); // tl
  Vec3F p1 = transform_3f(v3f(1.0f, 1.0f, 1.0f), xform); // tr
  Vec3F p2 = transform_3f(v3f(1.0f, 0.0f, 1.0f), xform); // br
  Vec3F p3 = transform_3f(v3f(0.0f, 0.0f, 1.0f), xform); // bl

  r_push_vertex(renderer, v4f(p0.x, p0.y, p0.z, 0.0f), tint, V4F_ZERO, v4f(0, 1, 0, 0));
  r_push_vertex(renderer, v4f(p1.x, p1.y, p1.z, 0.0f), tint, V4F_ZERO, v4f(1, 1, 0, 0));
  r_push_vertex(renderer, v4f(p2.x, p2.y, p2.z, 0.0f), tint, V4F_ZERO, v4f(1, 0, 0, 0));
  r_push_vertex(renderer, v4f(p3.x, p3.y, p3.z, 0.0f), tint, V4F_ZERO, v4f(0, 0, 0, 0));
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
