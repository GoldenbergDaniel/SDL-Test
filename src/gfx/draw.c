#include "glad/glad.h"
#include "base/base_common.h"
#include "base/base_arena.h"

#include "../global.h"
#include "gl_render.h"
#include "shaders.h"
#include "draw.h"

typedef D_Object Object;
typedef D_Renderer Renderer;
typedef R_Vertex Vertex;
typedef R_Shader Shader;
typedef R_Texture2D Texture2D;
typedef R_GL_VAO GL_VAO;

extern Global *GLOBAL;

void d_init_renderer(Renderer *renderer)
{
  Shader shader = r_gl_create_shader(shaders_vert_src, shaders_frag_src);

  SCOPE("Triangle State")
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

    GL_VAO vao = r_gl_create_vertex_array(2);
    u32 vbo = r_gl_create_vertex_buffer(&vertices, sizeof (vertices));
    u32 ibo = r_gl_create_index_buffer(&indices, sizeof (indices));
    r_gl_set_vertex_attribute(&vao, GL_FLOAT, 3); // position
    r_gl_set_vertex_attribute(&vao, GL_FLOAT, 3); // color

    renderer->triangle.vao = vao;
    renderer->triangle.vbo = vbo;
    renderer->triangle.ibo = ibo;
    renderer->triangle.shader = shader;

    r_gl_unbind_vertex_array();
  }

  SCOPE("Rectangle State")
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

    GL_VAO vao = r_gl_create_vertex_array(2);
    u32 vbo = r_gl_create_vertex_buffer(&vertices, sizeof (vertices));
    u32 ibo = r_gl_create_index_buffer(&indices, sizeof (indices));
    r_gl_set_vertex_attribute(&vao, GL_FLOAT, 3); // position
    r_gl_set_vertex_attribute(&vao, GL_FLOAT, 3); // color

    renderer->rectangle.vao = vao;
    renderer->rectangle.vbo = vbo;
    renderer->rectangle.ibo = ibo;
    renderer->rectangle.shader = shader;

    r_gl_unbind_vertex_array();
  }
}

void d_clear(Vec4F color)
{
  r_gl_clear_screen(color);
}

void d_triangle(Mat3x3F xform, Vec4F color)
{
  Renderer *renderer = GLOBAL->renderer;
  Object object = renderer->triangle;

  Mat3x3F projection = orthographic_3x3f(0.0f, W_WIDTH, 0.0f, W_HEIGHT);
  Mat3x3F transform = mul_3x3f(mul_3x3f(projection, *renderer->camera), xform);
  r_gl_bind_shader(&object.shader);
  r_gl_set_uniform_3x3f(&object.shader, "u_xform", transform);
  r_gl_set_uniform_4f(&object.shader, "u_color", color);

  r_gl_bind_vertex_array(&object.vao);
  r_gl_draw_triangles(3);
  r_gl_unbind_vertex_array();
}

void d_rectangle(Mat3x3F xform, Vec4F color)
{
  Renderer *renderer = GLOBAL->renderer;
  Object object = renderer->rectangle;

  Mat3x3F projection = orthographic_3x3f(0.0f, W_WIDTH, 0.0f, W_HEIGHT);
  Mat3x3F transform = mul_3x3f(mul_3x3f(projection, *renderer->camera), xform);
  r_gl_bind_shader(&object.shader);
  r_gl_set_uniform_3x3f(&object.shader, "u_xform", transform);
  r_gl_set_uniform_4f(&object.shader, "u_color", color);

  r_gl_bind_vertex_array(&object.vao);
  r_gl_draw_triangles(6);
  r_gl_unbind_vertex_array();
}
