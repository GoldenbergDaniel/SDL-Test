#pragma once

#include <assert.h>
#include "glad/glad.h"

#include "base/base_common.h"
#include "base/base_math.h"

typedef struct GL_Vertex GL_Vertex;
struct GL_Vertex
{
  f32 position[3];
  f32 color[3];
};

typedef struct GL_VertexLayout GL_VertexLayout;
struct GL_VertexLayout
{
  u32 index;
  u32 count;
  u32 data_type;
  bool normalized : 1;
  u32 stride;
  void *first;
};

typedef struct GL_Object GL_Object;
struct GL_Object
{
  u32 id;
  u8 attrib_count;
  u8 attrib_index;
};

typedef struct GL_Shader GL_Shader;
struct GL_Shader
{
  u32 id;
};

typedef struct GL_Texture2D GL_Texture2D;
struct GL_Texture2D
{
  u32 id;
  i32 width;
  i32 height;
  i32 num_channels;
  u8 *data;
};

#define DEBUG

#ifdef DEBUG
#define GL_ASSERT(call) \
  _gl_clear_error(); \
  call; \
  ASSERT(!_gl_check_error())
#else
#define GL_ASSERT(call) \
  call;
#endif

bool _gl_check_error(void);
void _gl_clear_error(void);

// @Buffer ==================================================================================

GL_Object gl_create_vertex_buffer(void *data, u32 size);
GL_Object gl_create_index_buffer(void *data, u32 size);
void gl_bind_vertex_buffer(GL_Object *buffer);
void gl_unbind_vertex_buffer(void);
void gl_bind_index_buffer(GL_Object *buffer);
void gl_unbind_index_buffer(void);

// @VertexArray =============================================================================

GL_Object gl_create_vertex_array(u8 attrib_count);
void gl_bind_vertex_array(GL_Object *vertex_array);
void gl_unbind_vertex_array(void);
void gl_set_vertex_layout(GL_Object *vertex_array, u32 data_type, u32 count);
void gl_bind_vertex_layout(GL_VertexLayout *layout);

// @Shader ==================================================================================

GL_Shader gl_create_shader(const i8 *vert_src, const i8 *frag_src);
void gl_bind_shader(GL_Shader *shader);
void gl_unbind_shader(void);
i32 gl_set_uniform_1u(GL_Shader *shader, i8 *name, u32 val);
i32 gl_set_uniform_1i(GL_Shader *shader, i8 *name, i32 val);
i32 gl_set_uniform_1f(GL_Shader *shader, i8 *name, f32 val);
i32 gl_set_uniform_2f(GL_Shader *shader, i8 *name, Vec2F vec);
i32 gl_set_uniform_3f(GL_Shader *shader, i8 *name, Vec3F vec);
i32 gl_set_uniform_4f(GL_Shader *shader, i8 *name, Vec4F vec);
i32 gl_set_uniform_3x3f(GL_Shader *shader, i8 *name, Mat3x3F mat);
i32 gl_set_uniform_4x4f(GL_Shader *shader, i8 *name, Mat4x4F mat);

// @Texture2D ===============================================================================

GL_Texture2D gl_create_texture2d(const i8 *path);
void gl_bind_texture2d(GL_Texture2D *texture);
void gl_unbind_texture2d(void);
