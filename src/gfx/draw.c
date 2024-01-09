#include "glad/glad.h"
#include "base/base_common.h"
#include "base/base_arena.h"

#include "../game.h"
#include "../global.h"
#include "render.h"
#include "shaders.h"
#include "draw.h"

typedef D_Resources Resources;
typedef D_TextureID TextureID;
typedef D_Renderer Renderer;
typedef D_Prefab Prefab;

extern Global *GLOBAL;

// Assets ======================================================================================

Resources d_load_resources(Arena *arena, const String path)
{
  Resources resources = {0};
  resources.textures = arena_alloc(arena, sizeof (R_Texture) * D_TEXTURE_COUNT);

#ifdef DEBUG
  Texture texture = r_gl_create_texture("../res/texture/sprites.png");
  resources.textures[0] = texture;
#else
  R_Texture texture = r_create_texture("res/texture/sprites.png");
#endif

  resources.textures[0] = texture;

  return resources;
}

// Renderer ====================================================================================

Renderer d_create_renderer(void)
{
  Renderer renderer = {0};

  SCOPE("Triangle")
  {
    R_Vertex vertices[3] = 
    {
      {{-5.0f,  5.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}, // top left
      {{ 10.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}, // right
      {{-5.0f, -5.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}  // bottom left
    };

    u32 indices[3] =
    {
      0, 1, 2
    };

    R_VAO vao = r_create_vertex_array(3);
    u32 vbo = r_create_vertex_buffer(&vertices, sizeof (vertices), FALSE);
    u32 ibo = r_create_index_buffer(&indices, sizeof (indices), FALSE);
    r_push_vertex_attribute(&vao, GL_FLOAT, 4); // position
    r_push_vertex_attribute(&vao, GL_FLOAT, 4); // color

    renderer.triangle.vao = vao;
    renderer.triangle.vbo = vbo;
    renderer.triangle.ibo = ibo;

    R_Shader shader = r_create_shader(primitive_vert_src, primitive_frag_src);
    renderer.triangle.shader = shader;

    glBindVertexArray(0);
  }

  SCOPE("Rectangle")
  {
    R_Vertex vertices[4] = 
    {
      {{-5.0f,  5.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}, // top left
      {{ 5.0f,  5.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}, // top right
      {{ 5.0f, -5.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}, // bottom right
      {{-5.0f, -5.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}  // bottom left
    };

    u32 indices[6] = 
    {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
    };

    R_VAO vao = r_create_vertex_array(3);
    u32 vbo = r_create_vertex_buffer(&vertices, sizeof (vertices), FALSE);
    u32 ibo = r_create_index_buffer(&indices, sizeof (indices), FALSE);
    r_push_vertex_attribute(&vao, GL_FLOAT, 4); // position
    r_push_vertex_attribute(&vao, GL_FLOAT, 4); // color

    renderer.rectangle.vao = vao;
    renderer.rectangle.vbo = vbo;
    renderer.rectangle.ibo = ibo;

    R_Shader shader = r_create_shader(primitive_vert_src, primitive_frag_src);
    renderer.rectangle.shader = shader;

    glBindVertexArray(0);
  }

  SCOPE("Sprite")
  {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    R_Vertex vertices[4] = 
    {
      {{-5.0f,  5.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}, // top left
      {{ 5.0f,  5.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // top right
      {{ 5.0f, -5.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // bottom right
      {{-5.0f, -5.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}  // bottom left
    };

    u32 indices[6] = 
    {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle 
    };

    R_VAO vao = r_create_vertex_array(3);
    u32 vbo = r_create_vertex_buffer(&vertices, sizeof (vertices), TRUE);
    u32 ibo = r_create_index_buffer(&indices, sizeof (indices), FALSE);
    r_push_vertex_attribute(&vao, GL_FLOAT, 4); // position
    r_push_vertex_attribute(&vao, GL_FLOAT, 4); // color
    r_push_vertex_attribute(&vao, GL_FLOAT, 4); // uv

    renderer.sprite.vao = vao;
    renderer.sprite.vbo = vbo;
    renderer.sprite.ibo = ibo;

    R_Shader shader = r_create_shader(sprite_vert_src, sprite_frag_src);
    renderer.sprite.shader = shader;

    glBindVertexArray(0);
  }

  return renderer;
}

inline
void d_clear(Vec4F color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void d_draw_triangle(Mat3x3F xform, Vec4F color)
{
  Prefab triangle = GLOBAL->renderer.triangle;

  glUseProgram(triangle.shader.id);
  r_set_uniform_3x3f(&triangle.shader, "u_xform", xform);
  r_set_uniform_4f(&triangle.shader, "u_color", color);

  glBindVertexArray(triangle.vao.id);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);
}

void d_draw_rectangle(Mat3x3F xform, Vec4F color)
{
  Prefab rect = GLOBAL->renderer.rectangle;

  glUseProgram(rect.shader.id);
  r_set_uniform_3x3f(&rect.shader, "u_xform", xform);
  r_set_uniform_4f(&rect.shader, "u_color", color);

  glBindVertexArray(rect.vao.id);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}

void d_draw_sprite(Mat3x3F xform, Vec4F color, TextureID tex_id)
{
  // fix y-coord
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

  r_update_vertex_buffer((f32 *) top_left.e, sizeof (Vec4F), sizeof (Vec4F) * 2);
  r_update_vertex_buffer((f32 *) top_right.e, sizeof (Vec4F), sizeof (Vec4F) * 5);
  r_update_vertex_buffer((f32 *) bot_right.e, sizeof (Vec4F), sizeof (Vec4F) * 8);
  r_update_vertex_buffer((f32 *) bot_left.e, sizeof (Vec4F), sizeof (Vec4F) * 11);

  R_Texture *texture = &GLOBAL->resources.textures[0];
  r_bind_texture(texture, texture->id);

  Prefab sprite = GLOBAL->renderer.sprite;

  glUseProgram(sprite.shader.id);
  r_set_uniform_3x3f(&sprite.shader, "u_xform", xform);
  r_set_uniform_4f(&sprite.shader, "u_color", color);
  r_set_uniform_1i(&sprite.shader, "u_tex", texture->id);

  glBindVertexArray(sprite.vao.id);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}
