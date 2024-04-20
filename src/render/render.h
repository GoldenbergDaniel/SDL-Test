#pragma once

#include "../base/base_inc.h"

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
  i16 u_xform;
  i16 u_color;
  i16 u_tex;
};

typedef struct R_Texture R_Texture;
struct R_Texture
{
  u32 id;
  u8 slot;
  i32 width;
  i32 height;
};

typedef struct R_VAO R_VAO;
struct R_VAO
{
  u32 id;
  u8 attrib_count;
  u8 attrib_index;
};

typedef struct R_Renderer R_Renderer;
struct R_Renderer
{
  R_Vertex *vertices;
  u32 vertex_count;
  u32 vertex_capacity;

  u32 *indices;
  u32 index_count;

  R_VAO vao;
  u32 vbo;
  u32 ibo;

  R_Shader *shader;
  R_Texture *texture;

  Mat3x3F projection;
};

static R_Shader *R_NIL_SHADER = &(R_Shader) {0};
static R_Texture *R_NIL_TEXTURE = &(R_Texture) {0};

#define R_BLACK ((Vec4F) {0.0f, 0.0f, 0.0f, 1.0f})
#define R_WHITE ((Vec4F) {1.0f, 1.0f, 1.0f, 1.0f})

#define R_PROJECTION_ORTHO_TL 0 
#define R_PROJECTION_ORTHO_BL 1
#define R_PROJECTION_ORTHO_C 2

// @Buffer ///////////////////////////////////////////////////////////////////////////////

u32 r_create_vertex_buffer(void *data, u32 size, bool dynamic);
void r_update_vertex_buffer(void *data, u32 size, u32 offset);
u32 r_create_index_buffer(void *data, u32 size, bool dynamic);
void r_update_index_buffer(void *data, u32 size, u32 offset);

// @VAO //////////////////////////////////////////////////////////////////////////////////

R_VAO r_create_vertex_array(u8 attrib_count);
void r_push_vertex_attribute(R_VAO *vao, u32 count);

// @Shader ///////////////////////////////////////////////////////////////////////////////

R_Shader r_create_shader(const char *vert_src, const char *frag_src);
void r_set_uniform_1u(R_Shader *shader, i32 loc, u32 val);
void r_set_uniform_1i(R_Shader *shader, i32 loc, i32 val);
void r_set_uniform_1f(R_Shader *shader, i32 loc, f32 val);
void r_set_uniform_2f(R_Shader *shader, i32 loc, Vec2F val);
void r_set_uniform_3f(R_Shader *shader, i32 loc, Vec3F val);
void r_set_uniform_4f(R_Shader *shader, i32 loc, Vec4F val);
void r_set_uniform_3x3f(R_Shader *shader, i32 loc, Mat3x3F val);

// @Texture //////////////////////////////////////////////////////////////////////////////

R_Texture r_create_texture(String path);

// @Rendering ////////////////////////////////////////////////////////////////////////////

R_Renderer r_create_renderer(u32 vertex_capacity, Arena *arena);
void r_push_vertex(R_Renderer *renderer, Vec4F pos, Vec4F color, Vec4F uv);
void r_push_quad_indices(R_Renderer *renderer);
void r_use_shader(R_Renderer *renderer, R_Shader *shader);
void r_use_texture(R_Renderer *renderer, R_Texture *texture);
void r_flush(R_Renderer *renderer);
