#pragma once

#include "glad/glad.h"
#include "base/base_common.h"
#include "base/base_arena.h"
#include "base/base_math.h"

typedef struct R_Vertex R_Vertex;
struct R_Vertex
{
  Vec4F position;
  Vec4F tint;
  Vec4F uv;
};

typedef struct R_Shader R_Shader;
struct R_Shader
{
  u32 id;
};

typedef struct R_Texture R_Texture;
struct R_Texture
{
  u32 id;
  i32 width;
  i32 height;
  i32 channel_count;
  u8 *data;
};

typedef struct R_VAO R_VAO;
struct R_VAO
{
  u32 id;
  u8 attrib_count;
  u8 attrib_index;
};

// @Buffer =====================================================================================

u32 r_create_vertex_buffer(void *data, u32 size, bool dynamic);
void r_update_vertex_buffer(void *data, u32 size, u32 offset);
u32 r_create_index_buffer(void *data, u32 size, bool dynamic);

// @VAO ========================================================================================

R_VAO r_create_vertex_array(u8 attrib_count);
void r_push_vertex_attribute(R_VAO *vao, u32 data_type, u32 count);

// @Shader =====================================================================================

R_Shader r_create_shader(const char *vert_src, const char *frag_src);
void r_set_uniform_1u(R_Shader *shader, const char *name, u32 val);
void r_set_uniform_1i(R_Shader *shader, const char *name, i32 val);
void r_set_uniform_1f(R_Shader *shader, const char *name, f32 val);
void r_set_uniform_2f(R_Shader *shader, const char *name, Vec2F val);
void r_set_uniform_3f(R_Shader *shader, const char *name, Vec3F val);
void r_set_uniform_4f(R_Shader *shader, const char *name, Vec4F val);
void r_set_uniform_3x3f(R_Shader *shader, const char *name, Mat3x3F val);

// @Texture ====================================================================================

R_Texture r_create_texture(const char *path);
void r_bind_texture(R_Texture *texture, u32 slot);

// @Rendering ==================================================================================

typedef struct R_BatchRenderer R_BatchRenderer;
struct R_BatchRenderer
{
  R_Vertex *vertices;
  u32 vertex_count;
  u32 vertex_capacity;

  R_VAO vao;
  u32 vbo;
  u32 ibo;

  R_Shader shader;
  R_Texture *texture;
};

R_BatchRenderer r_create_batch_renderer(u32 vertex_capacity, Arena *arena);
void r_push_vertex(R_BatchRenderer *renderer, Vec2F pos, Vec2F uv);
void r_flush(R_BatchRenderer *renderer);
