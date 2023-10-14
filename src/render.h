#pragma once

#include <assert.h>
#include "glad/glad.h"

#include "base/base_common.h"
#include "base/base_math.h"

typedef struct R_Vertex R_Vertex;
struct R_Vertex
{
  f32 position[3];
  f32 color[3];
};

typedef struct R_VertexLayout R_VertexLayout;
struct R_VertexLayout
{
  u32 index;
  u32 count;
  u32 data_type;
  bool normalized : 1;
  u32 stride;
  void *first;
};

typedef struct R_Object R_Object;
struct R_Object
{
  u32 id;
  u8 attrib_count;
  u8 attrib_index;
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
  i32 num_channels;
  u8 *data;
};

#define DEBUG

#ifdef DEBUG
#define R_ASSERT(call) \
  _r_clear_error(); \
  call; \
  ASSERT(!_r_check_error())
#else
#define R_ASSERT(call) \
  call;
#endif

bool _r_check_error(void);
void _r_clear_error(void);

// @Buffer ==================================================================================

R_Object r_create_vertex_buffer(void *data, u32 size);
R_Object r_create_index_buffer(void *data, u32 size);
void r_bind_vertex_buffer(R_Object *buffer);
void r_unbind_vertex_buffer(void);
void r_bind_index_buffer(R_Object *buffer);
void r_unbind_index_buffer(void);

// @VertexArray =============================================================================

R_Object r_create_vertex_array(u8 attrib_count);
void r_bind_vertex_array(R_Object *vertex_array);
void r_unbind_vertex_array(void);
R_VertexLayout r_create_vertex_layout(R_Object *vertex_array, u32 type, u32 count);
void r_bind_vertex_layout(R_VertexLayout *layout);

// @Shader ==================================================================================

R_Shader r_create_shader(const i8 *vert_src, const i8 *frag_src);
void r_bind_shader(R_Shader *shader);
void r_unbind_shader(void);
i32 r_set_uniform_1u(R_Shader *shader, i8 *name, u32 val);
i32 r_set_uniform_1i(R_Shader *shader, i8 *name, i32 val);
i32 r_set_uniform_1f(R_Shader *shader, i8 *name, f32 val);
i32 r_set_uniform_2f(R_Shader *shader, i8 *name, Vec2F vec);
i32 r_set_uniform_3f(R_Shader *shader, i8 *name, Vec3F vec);
i32 r_set_uniform_4f(R_Shader *shader, i8 *name, Vec4F vec);
i32 r_set_uniform_3x3f(R_Shader *shader, i8 *name, Mat3x3F mat);
i32 r_set_uniform_4x4f(R_Shader *shader, i8 *name, Mat4x4F mat);

// @Texture2D ===============================================================================

R_Texture2D r_create_texture2d(const i8 *path);
void r_bind_texture2d(R_Texture2D *texture);
void r_unbind_texture2d(void);
