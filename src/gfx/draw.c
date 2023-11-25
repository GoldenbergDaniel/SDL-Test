#include "glad/glad.h"
#include "base/base_common.h"
#include "base/base_arena.h"

#include "../game.h"
#include "gl_render.h"
#include "shaders.h"
#include "draw.h"

typedef R_Vertex Vertex;
typedef R_Shader Shader;
typedef R_Texture2D Texture2D;
typedef R_GL_VAO GL_VAO;

extern Global *GLOBAL;

void d_init_renderer(Renderer *renderer)
{
  SCOPE("Triangle")
  {
    Vertex vertices[3] = 
    {
      {{-10.0f,  10.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}, // top left
      {{ 20.0f,  0.0f,  1.0f},  {0.0f, 0.0f, 0.0f}}, // right
      {{-10.0f, -10.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}  // bottom left
    };

    u32 indices[3] = 
    {
      0, 1, 2
    };

    GL_VAO vao = r_gl_create_vertex_array(3);
    u32 vbo = r_gl_create_vertex_buffer(&vertices, sizeof (vertices));
    u32 ibo = r_gl_create_index_buffer(&indices, sizeof (indices));
    r_gl_set_vertex_attribute(&vao, GL_FLOAT, 3); // position
    r_gl_set_vertex_attribute(&vao, GL_FLOAT, 3); // color

    renderer->triangle.vao = vao;
    renderer->triangle.vbo = vbo;
    renderer->triangle.ibo = ibo;

    Shader shader = r_gl_create_shader(primitive_vert_src, primitive_frag_src);
    renderer->triangle.shader = shader;

    r_gl_unbind_vertex_array();
  }

  SCOPE("Rectangle")
  {
    Vertex vertices[4] = 
    {
      {{-10.0f,  10.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}, // top left
      {{ 10.0f,  10.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}, // top right
      {{ 10.0f, -10.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}, // bottom right
      {{-10.0f, -10.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}  // bottom left
    };

    u32 indices[6] = 
    {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
    };

    GL_VAO vao = r_gl_create_vertex_array(3);
    u32 vbo = r_gl_create_vertex_buffer(&vertices, sizeof (vertices));
    u32 ibo = r_gl_create_index_buffer(&indices, sizeof (indices));
    r_gl_set_vertex_attribute(&vao, GL_FLOAT, 3); // position
    r_gl_set_vertex_attribute(&vao, GL_FLOAT, 3); // color

    renderer->rectangle.vao = vao;
    renderer->rectangle.vbo = vbo;
    renderer->rectangle.ibo = ibo;

    Shader shader = r_gl_create_shader(primitive_vert_src, primitive_frag_src);
    renderer->rectangle.shader = shader;

    r_gl_unbind_vertex_array();
  }

  SCOPE("Sprite")
  {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    Vertex vertices[4] = 
    {
      {{-10.0f,  10.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // top left
      {{ 10.0f,  10.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}}, // top right
      {{ 10.0f, -10.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // bottom right
      {{-10.0f, -10.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}  // bottom left
    };

    u32 indices[6] = 
    {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle 
    };

    GL_VAO vao = r_gl_create_vertex_array(3);
    u32 vbo = r_gl_create_vertex_buffer(&vertices, sizeof (vertices));
    u32 ibo = r_gl_create_index_buffer(&indices, sizeof (indices));
    r_gl_set_vertex_attribute(&vao, GL_FLOAT, 3); // position
    r_gl_set_vertex_attribute(&vao, GL_FLOAT, 3); // color
    r_gl_set_vertex_attribute(&vao, GL_FLOAT, 3); // texture coordinate

    renderer->sprite.vao = vao;
    renderer->sprite.vbo = vbo;
    renderer->sprite.ibo = ibo;

    Shader shader = r_gl_create_shader(sprite_vert_src, sprite_frag_src);
    renderer->sprite.shader = shader;

    Texture2D texture = r_gl_create_texture2d("res/player.png");
    u32 slot = 0;
    renderer->sprite.texture2d = texture;
    renderer->sprite.texture2d_slot = slot;
    r_gl_bind_texture2d(&texture, slot);
    r_gl_set_uniform_1i(&shader, "u_tex", slot);
    r_gl_unbind_texture2d();

    r_gl_unbind_vertex_array();
  }
}

inline
void d_clear(Vec4F color)
{
  r_gl_clear_screen(color);
}

inline
void d_triangle(Mat3x3F xform, Vec4F color)
{
  RenderState object = GLOBAL->renderer->triangle;

  r_gl_bind_shader(&object.shader);
  r_gl_set_uniform_3x3f(&object.shader, "u_xform", xform);
  r_gl_set_uniform_4f(&object.shader, "u_color", color);

  r_gl_bind_vertex_array(&object.vao);
  r_gl_draw_triangles(3);
  
  r_gl_unbind_vertex_array();
  r_gl_unbind_shader();
}

inline
void d_rectangle(Mat3x3F xform, Vec4F color)
{
  RenderState object = GLOBAL->renderer->rectangle;

  r_gl_bind_shader(&object.shader);
  r_gl_set_uniform_3x3f(&object.shader, "u_xform", xform);
  r_gl_set_uniform_4f(&object.shader, "u_color", color);

  r_gl_bind_vertex_array(&object.vao);
  r_gl_draw_triangles(6);

  r_gl_unbind_vertex_array();
  r_gl_unbind_shader();
}

inline
void d_sprite(Mat3x3F xform, Vec4F color)
{
  RenderState object = GLOBAL->renderer->sprite;

  r_gl_bind_texture2d(&object.texture2d, object.texture2d_slot);

  r_gl_bind_shader(&object.shader);
  r_gl_set_uniform_3x3f(&object.shader, "u_xform", xform);
  r_gl_set_uniform_4f(&object.shader, "u_color", color);

  r_gl_bind_vertex_array(&object.vao);
  r_gl_draw_triangles(6);

  r_gl_unbind_vertex_array();
  r_gl_unbind_texture2d();
  r_gl_unbind_shader();
}
