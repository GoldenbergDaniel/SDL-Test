#include "base_common.h"
#include "render.h"
#include "draw.h"

typedef R_Object Object;
typedef R_Shader Shader;

// @Draw ====================================================================================

void d_clear(Vec4F color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void d_draw_rect(D_Stream *stream, Mat3x3F xform, Vec4F color)
{
  Mat3x3F projection = orthographic_3x3f(0.0f, W_WIDTH, 0.0f, W_HEIGHT);
  // stream->camera = mul_3x3f(m3x3f(-1.0f), stream->camera);
  Mat3x3F scp = mul_3x3f(mul_3x3f(projection, stream->camera), xform);
  
  r_set_uniform_3x3f(&stream->shader, "u_xform", scp);
  r_set_uniform_4f(&stream->shader, "u_color", color);
  r_bind_shader(&stream->shader);
  r_bind_vertex_array(&stream->vao);
  R_ASSERT(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL));
}
