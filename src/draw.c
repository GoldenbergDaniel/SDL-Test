#include <stdio.h>

#include "base/base_common.h"
#include "base/base_arena.h"
#include "util.h"
#include "gl_render.h"
#include "shaders.h"
#include "draw.h"

typedef D_Object Object;
typedef D_Renderer Renderer;

extern Global *GLOBAL;

void d_init_renderer(D_Renderer *renderer)
{
  GL_Shader shader = gl_create_shader(shaders_vert_src, shaders_frag_src);

  SCOPE("Triangle")
  {
    GL_Vertex vertices[3] = 
    {
      {{-10.0f,  10.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}, // top left
      {{ 20.0f,  0.0f,  1.0f},  {0.0f, 0.0f, 0.0f}}, // right
      {{-10.0f, -10.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}  // bottom left
    };

    u16 indices[3] = 
    {
      0, 1, 2
    };

    GL_Object vao = gl_create_vertex_array(2);
    GL_Object vbo = gl_create_vertex_buffer(&vertices, sizeof (vertices));
    GL_Object ibo = gl_create_index_buffer(&indices, sizeof (indices));
    gl_set_vertex_layout(&vao, GL_FLOAT, 3);
    gl_set_vertex_layout(&vao, GL_FLOAT, 3);

    renderer->triangle.vao = vao;
    renderer->triangle.vbo = vbo;
    renderer->triangle.ibo = ibo;
    renderer->triangle.shader = shader;

    gl_unbind_vertex_array();
  }

  SCOPE("Rectangle")
  {
    GL_Vertex vertices[4] = 
    {
      {{-10.0f,  10.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}, // top left
      {{ 10.0f,  10.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}, // top right
      {{ 10.0f, -10.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}, // bottom right
      {{-10.0f, -10.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}  // bottom left
    };

    u16 indices[6] = 
    {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
    };

    GL_Object vao = gl_create_vertex_array(2);
    GL_Object vbo = gl_create_vertex_buffer(&vertices, sizeof (vertices));
    GL_Object ibo = gl_create_index_buffer(&indices, sizeof (indices));
    gl_set_vertex_layout(&vao, GL_FLOAT, 3);
    gl_set_vertex_layout(&vao, GL_FLOAT, 3);

    renderer->rectangle.vao = vao;
    renderer->rectangle.vbo = vbo;
    renderer->rectangle.ibo = ibo;
    renderer->rectangle.shader = shader;

    gl_unbind_vertex_array();
  }
}

void d_clear(Vec4F color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void d_draw_triangle(Mat3x3F xform, Vec4F color)
{
  Renderer *renderer = GLOBAL->renderer;
  Object object = renderer->triangle;

  Mat3x3F projection = orthographic_3x3f(0.0f, W_WIDTH, 0.0f, W_HEIGHT);
  Mat3x3F transform = mul_3x3f(mul_3x3f(projection, *renderer->camera), xform);
  gl_bind_shader(&object.shader);
  gl_set_uniform_3x3f(&object.shader, "u_xform", transform);
  gl_set_uniform_4f(&object.shader, "u_color", color);

  gl_bind_vertex_array(&object.vao);
  GL_ASSERT(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL));
  gl_unbind_vertex_array();
}

void d_draw_rectangle(Mat3x3F xform, Vec4F color)
{
  Renderer *renderer = GLOBAL->renderer;
  Object object = renderer->rectangle;

  Mat3x3F projection = orthographic_3x3f(0.0f, W_WIDTH, 0.0f, W_HEIGHT);
  Mat3x3F transform = mul_3x3f(mul_3x3f(projection, *renderer->camera), xform);
  gl_bind_shader(&object.shader);
  gl_set_uniform_3x3f(&object.shader, "u_xform", transform);
  gl_set_uniform_4f(&object.shader, "u_color", color);

  gl_bind_vertex_array(&object.vao);
  GL_ASSERT(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL));
  gl_unbind_vertex_array();
}
