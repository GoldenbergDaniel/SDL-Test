#include "base/base_common.h"
#include "base/base_arena.h"
#include "game.h"
#include "render.h"
#include "shaders.h"
#include "draw.h"

typedef D_Object Object;
typedef D_Renderer Renderer;
typedef R_Shader Shader;

extern State *state;

void d_init_renderer(D_Renderer *renderer)
{
  R_Shader shader = r_create_shader(shaders_vert_src, shaders_frag_src);
  renderer->camera = translate_3x3f(0.0f, 0.0f);

  // TRIANGLE
  {
    R_Vertex vertices[3] = 
    {
      {{-10.0f,  10.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}, // top left
      {{ 20.0f,  0.0f,  1.0f},  {0.0f, 0.0f, 0.0f}}, // right
      {{-10.0f, -10.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}  // bottom left
    };

    u16 indices[3] = 
    {
      0, 1, 2
    };

    R_Object vao = r_create_vertex_array(2);
    R_Object vbo = r_create_vertex_buffer(&vertices, sizeof (vertices));
    R_Object ibo = r_create_index_buffer(&indices, sizeof (indices));
    R_VertexLayout vl = r_create_vertex_layout(&vao, GL_FLOAT, 3);
    R_VertexLayout cl = r_create_vertex_layout(&vao, GL_FLOAT, 3);

    renderer->triangle.vao = vao;
    renderer->triangle.vbo = vbo;
    renderer->triangle.ibo = ibo;
    renderer->triangle.shader = shader;
    renderer->triangle.v_layout = vl;
    renderer->triangle.v_layout = cl;

    r_unbind_vertex_buffer();
    r_unbind_vertex_array();
    r_unbind_index_buffer();
  }

  // RECTANGLE
  {
    R_Vertex vertices[4] = 
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

    R_Object vao = r_create_vertex_array(2);
    R_Object vbo = r_create_vertex_buffer(&vertices, sizeof (vertices));
    R_Object ibo = r_create_index_buffer(&indices, sizeof (indices));
    R_VertexLayout vl = r_create_vertex_layout(&vao, GL_FLOAT, 3);
    R_VertexLayout cl = r_create_vertex_layout(&vao, GL_FLOAT, 3);

    renderer->rectangle.shader = shader;
    renderer->rectangle.vao = vao;
    renderer->rectangle.vbo = vbo;
    renderer->rectangle.ibo = ibo;
    renderer->rectangle.v_layout = vl;
    renderer->rectangle.v_layout = cl;

    r_unbind_vertex_buffer();
    r_unbind_vertex_array();
    r_unbind_index_buffer();
  }
}

void d_clear(Vec4F color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void d_draw_triangle(Mat3x3F xform, Vec4F color)
{
  Renderer *renderer = state->renderer;
  Object triangle = renderer->triangle;

  Mat3x3F projection = orthographic_3x3f(0.0f, W_WIDTH, 0.0f, W_HEIGHT);
  Mat3x3F scp = mul_3x3f(mul_3x3f(projection, renderer->camera), xform);
  r_set_uniform_3x3f(&triangle.shader, "u_xform", scp);
  r_set_uniform_4f(&triangle.shader, "u_color", color);

  r_bind_shader(&triangle.shader);
  r_bind_vertex_array(&triangle.vao);
  r_bind_vertex_buffer(&triangle.vbo);
  r_bind_index_buffer(&triangle.ibo);
  r_bind_vertex_layout(&triangle.v_layout);
  r_bind_vertex_layout(&triangle.c_layout);

  R_ASSERT(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL));
}

void d_draw_rectangle(Mat3x3F xform, Vec4F color)
{
  Renderer *renderer = state->renderer;
  Object rectangle = renderer->rectangle;

  Mat3x3F projection = orthographic_3x3f(0.0f, W_WIDTH, 0.0f, W_HEIGHT);
  Mat3x3F scp = mul_3x3f(mul_3x3f(projection, renderer->camera), xform);
  r_set_uniform_3x3f(&rectangle.shader, "u_xform", scp);
  r_set_uniform_4f(&rectangle.shader, "u_color", color);

  r_bind_shader(&rectangle.shader);
  r_bind_vertex_array(&rectangle.vao);
  r_bind_vertex_buffer(&rectangle.vbo);
  r_bind_index_buffer(&rectangle.ibo);
  r_bind_vertex_layout(&rectangle.v_layout);
  r_bind_vertex_layout(&rectangle.c_layout);

  R_ASSERT(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL));
}
