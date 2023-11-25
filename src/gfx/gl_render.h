#pragma once

#include "glad/glad.h"
#include "base/base_common.h"
#include "base/base_math.h"

typedef struct R_Vertex R_Vertex;
struct R_Vertex
{
  Vec3F position;
  Vec3F color;
  Vec3F texture_coordinate;
};

typedef struct R_Shader R_Shader;
struct R_Shader
{
  u32 id;
};

typedef struct R_Texture2D R_Texture2D;
struct R_Texture2D
{
  u32 id;
  i32 width;
  i32 height;
  i32 channel_count;
  u8 *data;
};

typedef struct R_GL_VAO R_GL_VAO;
struct R_GL_VAO
{
  u32 id;
  u8 attrib_count;
  u8 attrib_index;
};

#ifdef DEBUG
#define GL_ASSERT(call) \
  _r_gl_clear_error(); \
  call; \
  ASSERT(!_r_gl_check_error())
#else
#define GL_ASSERT(call) \
  call;
#endif

bool _r_gl_check_error(void);
void _r_gl_clear_error(void);

// @Buffer =====================================================================================

u32 r_gl_create_vertex_buffer(void *data, u32 size);
u32 r_gl_create_index_buffer(void *data, u32 size);
void r_gl_bind_vertex_buffer(u32 id);
void r_gl_unbind_vertex_buffer(void);
void r_gl_bind_index_buffer(u32 id);
void r_gl_unbind_index_buffer(void);

// @VAO ========================================================================================

R_GL_VAO r_gl_create_vertex_array(u8 attrib_count);
void r_gl_bind_vertex_array(R_GL_VAO *vao);
void r_gl_unbind_vertex_array(void);
void r_gl_set_vertex_attribute(R_GL_VAO *vao, GLenum data_type, u32 count);

// @Shader =====================================================================================

R_Shader r_gl_create_shader(const i8 *vert_src, const i8 *frag_src);
void r_gl_bind_shader(R_Shader *shader);
void r_gl_unbind_shader(void);
i32 r_gl_set_uniform_1u(R_Shader *shader, i8 *name, u32 v);
i32 r_gl_set_uniform_1i(R_Shader *shader, i8 *name, i32 v);
i32 r_gl_set_uniform_1f(R_Shader *shader, i8 *name, f32 v);
i32 r_gl_set_uniform_2f(R_Shader *shader, i8 *name, Vec2F v);
i32 r_gl_set_uniform_3f(R_Shader *shader, i8 *name, Vec3F v);
i32 r_gl_set_uniform_4f(R_Shader *shader, i8 *name, Vec4F v);
i32 r_gl_set_uniform_3x3f(R_Shader *shader, i8 *name, Mat3x3F m);
i32 r_gl_set_uniform_4x4f(R_Shader *shader, i8 *name, Mat4x4F m);

// @Texture2D ==================================================================================

R_Texture2D r_gl_create_texture2d(const i8 *path);
void r_gl_bind_texture2d(R_Texture2D *texture, u32 slot);
void r_gl_unbind_texture2d(void);

// @Draw =======================================================================================

void r_gl_clear_screen(Vec4F color);
void r_gl_draw_triangles(u32 vertex_count);
void r_gl_draw_triangles_instanced(u32 vertex_count);
