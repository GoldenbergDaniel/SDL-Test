#include "glad/glad.h"
#include "base/base_common.h"
#include "base/base_arena.h"

#include "../game.h"
#include "gl_render.h"
#include "shaders.h"
#include "draw.h"

typedef D_Resources Resources;
typedef D_TextureID TextureID;
typedef D_Renderer Renderer;
typedef D_RenderState RenderState;

typedef R_Vertex Vertex;
typedef R_Shader Shader;
typedef R_Texture Texture;
typedef R_VAO VAO;

extern Global *GLOBAL;

// Assets ======================================================================================

Resources d_load_resources(Arena *arena, const String path)
{
  Resources resources = {0};
  resources.textures = arena_alloc(arena, sizeof (Texture) * D_TEXTURE_COUNT);
  resources.shaders = arena_alloc(arena, sizeof (Shader) * D_SHADER_COUNT);

  Texture texture = {0};

#ifdef DEBUG
  texture = r_gl_create_texture("../res/texture/sprites.png");
  resources.textures[0] = texture;
#else
    texture = r_gl_create_texture("res/texture/sprites.png");
#endif

  resources.textures[0] = texture;
  return resources;
}

Texture d_get_texture(const String name)
{
  Texture result = {0};

  return result;
}

Shader d_get_shader(const String name)
{
  Shader result = {0};
  
  return result;
}

// Renderer ====================================================================================

Renderer d_create_renderer(void)
{
  Renderer renderer = {0};

  SCOPE("Triangle")
  {
    Vertex vertices[3] = 
    {
      {{-5.0f,  5.0f, 1.0f},  {0.0f, 0.0f, 0.0f, 1.0f}}, // top left
      {{ 10.0f, 0.0f, 1.0f},  {0.0f, 0.0f, 0.0f, 1.0f}}, // right
      {{-5.0f, -5.0f, 1.0f},  {0.0f, 0.0f, 0.0f, 1.0f}}  // bottom left
    };

    u32 indices[3] = 
    {
      0, 1, 2
    };

    VAO vao = r_gl_create_vertex_array(3);
    u32 vbo = r_gl_create_vertex_buffer(&vertices, sizeof (vertices), FALSE);
    u32 ibo = r_gl_create_index_buffer(&indices, sizeof (indices));
    r_gl_add_vertex_attribute(&vao, GL_FLOAT, 4); // position
    r_gl_add_vertex_attribute(&vao, GL_FLOAT, 4); // color

    renderer.triangle.vao = vao;
    renderer.triangle.vbo = vbo;
    renderer.triangle.ibo = ibo;

    Shader shader = r_gl_create_shader(primitive_vert_src, primitive_frag_src);
    renderer.triangle.shader = shader;

    r_gl_unbind_vertex_array();
  }

  SCOPE("Rectangle")
  {
   Vertex vertices[4] = 
    {
      {{-5.0f,  5.0f, 1.0f},  {0.0f, 0.0f, 0.0f, 1.0f}}, // top left
      {{ 5.0f,  5.0f, 1.0f},  {0.0f, 0.0f, 0.0f, 1.0f}}, // top right
      {{ 5.0f, -5.0f, 1.0f},  {0.0f, 0.0f, 0.0f, 1.0f}}, // bottom right
      {{-5.0f, -5.0f, 1.0f},  {0.0f, 0.0f, 0.0f, 1.0f}}  // bottom left
    };

    u32 indices[6] = 
    {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
    };

    VAO vao = r_gl_create_vertex_array(3);
    u32 vbo = r_gl_create_vertex_buffer(&vertices, sizeof (vertices), FALSE);
    u32 ibo = r_gl_create_index_buffer(&indices, sizeof (indices));
    r_gl_add_vertex_attribute(&vao, GL_FLOAT, 4); // position
    r_gl_add_vertex_attribute(&vao, GL_FLOAT, 4); // color

    renderer.rectangle.vao = vao;
    renderer.rectangle.vbo = vbo;
    renderer.rectangle.ibo = ibo;

    Shader shader = r_gl_create_shader(primitive_vert_src, primitive_frag_src);
    renderer.rectangle.shader = shader;

    r_gl_unbind_vertex_array();
  }

  SCOPE("Sprite")
  {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    Vertex vertices[4] = 
    {
      {{-5.0f,  5.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}, // top left
      {{ 5.0f,  5.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0, 1.0}}, // top right
      {{ 5.0f, -5.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0, 0.0f}}, // bottom right
      {{-5.0f, -5.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}  // bottom left
    };

    u32 indices[6] = 
    {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle 
    };

    VAO vao = r_gl_create_vertex_array(3);
    u32 vbo = r_gl_create_vertex_buffer(&vertices, sizeof (vertices), TRUE);
    u32 ibo = r_gl_create_index_buffer(&indices, sizeof (indices));
    r_gl_add_vertex_attribute(&vao, GL_FLOAT, 4); // position
    r_gl_add_vertex_attribute(&vao, GL_FLOAT, 4); // color
    r_gl_add_vertex_attribute(&vao, GL_FLOAT, 4); // texture coordinate

    renderer.sprite.vao = vao;
    renderer.sprite.vbo = vbo;
    renderer.sprite.ibo = ibo;

    Shader shader = r_gl_create_shader(sprite_vert_src, sprite_frag_src);
    renderer.sprite.shader = shader;

    r_gl_unbind_vertex_array();
  }

  return renderer;
}

inline
void d_clear(Vec4F color)
{
  r_gl_clear_screen(color);
}

inline
void d_triangle(Mat3x3F xform, Vec4F color)
{
  RenderState state = GLOBAL->renderer.triangle;
  r_gl_bind_vertex_array(&state.vao);

  r_gl_bind_shader(&state.shader);
  r_gl_set_uniform_3x3f(&state.shader, "u_xform", xform);
  r_gl_set_uniform_4f(&state.shader, "u_color", color);

  r_gl_draw_triangles(3);
  
  // r_gl_unbind_vertex_array();
}

inline
void d_rectangle(Mat3x3F xform, Vec4F color)
{
  RenderState state = GLOBAL->renderer.rectangle;
  r_gl_bind_vertex_array(&state.vao);

  r_gl_bind_shader(&state.shader);
  r_gl_set_uniform_3x3f(&state.shader, "u_xform", xform);
  r_gl_set_uniform_4f(&state.shader, "u_color", color);

  r_gl_draw_triangles(6);

  // r_gl_unbind_vertex_array();
}

inline
void d_sprite(Mat3x3F xform, Vec4F color, TextureID tex_id)
{
  RenderState state = GLOBAL->renderer.sprite;
  r_gl_bind_vertex_array(&state.vao);

  // fix y-coord
  tex_id.y = (D_SPRITE_SHEET_HEIGHT / D_SPRITE_SHEET_SIZE) - tex_id.y - 1;

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

  r_gl_update_vertex_buffer((f32 *) top_left.e, sizeof (Vec4F), sizeof (Vec4F) * 2);
  r_gl_update_vertex_buffer((f32 *) top_right.e, sizeof (Vec4F), sizeof (Vec4F) * 5);
  r_gl_update_vertex_buffer((f32 *) bot_right.e, sizeof (Vec4F), sizeof (Vec4F) * 8);
  r_gl_update_vertex_buffer((f32 *) bot_left.e, sizeof (Vec4F), sizeof (Vec4F) * 11);

  R_Texture *texture = &GLOBAL->resources.textures[0];
  r_gl_bind_texture(texture, texture->id);

  r_gl_bind_shader(&state.shader);
  r_gl_set_uniform_3x3f(&state.shader, "u_xform", xform);
  r_gl_set_uniform_4f(&state.shader, "u_color", color);
  r_gl_set_uniform_1i(&state.shader, "u_tex", texture->id);

  r_gl_draw_triangles(6);

  // r_gl_unbind_vertex_array();
  // r_gl_unbind_texture();
}
