#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb/stb_image.h"

#include "base/base_common.h"
#include "base/base_math.h"
#include "render.h"
// #include "shaders.h"
#include "../game.h"
#include "../global.h"

#ifdef DEBUG
static void verify_shader(u32 id, u32 type);
#endif

#define WHITE ((Vec4F) {1.0f, 1.0f, 1.0f, 1.0f})
#define BLACK ((Vec4F) {0.0f, 0.0f, 0.0f, 1.0f})

extern Global *GLOBAL;
extern const char *primitive_vert_src;
extern const char *primitive_frag_src;
extern const char *sprite_vert_src;
extern const char *sprite_frag_src;

// @Buffer =====================================================================================

u32 r_create_vertex_buffer(void *data, u32 size, bool dynamic)
{
  u32 id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  GLenum draw_type = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
  glBufferData(GL_ARRAY_BUFFER, size, data, draw_type);

  return id;
}

inline
void r_update_vertex_buffer(void *data, u32 size, u32 offset)
{
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

u32 r_create_index_buffer(void *data, u32 size, bool dynamic)
{
  u32 id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  GLenum draw_type = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, draw_type);

  return id;
}

inline
void r_update_index_buffer(void *data, u32 size, u32 offset)
{
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

// @RAO ========================================================================================

R_VAO r_create_vertex_array(u8 attrib_count)
{
  u32 id;
  glGenVertexArrays(1, &id);
  glBindVertexArray(id);

  return (R_VAO) 
  {
    .id = id, 
    .attrib_count = attrib_count, 
    .attrib_index = 0,
  };
}

void r_push_vertex_attribute(R_VAO *vao, u32 data_type, u32 count)
{
  typedef struct Layout Layout;
  struct Layout
  {
    u32 index;
    u32 count;
    u32 data_type;
    bool normalized;
    u32 stride;
    void *offset;
  };

  u8 type_size;
  switch (data_type)
  {
    case GL_BYTE:  type_size = sizeof (i8); break;
    case GL_SHORT: type_size = sizeof (i16); break;
    case GL_INT:   type_size = sizeof (i32); break;
    case GL_FLOAT: type_size = sizeof (f32); break;
    default: assert(FALSE);
  }

  Layout layout = 
  {
    .index = vao->attrib_index,
    .count = count,
    .data_type = data_type,
    .normalized = FALSE,
    .stride = count * type_size * vao->attrib_count,
    .offset = (void *) (u64) (vao->attrib_index * count * type_size)
  };

  glVertexAttribPointer(
                        layout.index,
                        layout.count,
                        layout.data_type,
                        layout.normalized,
                        layout.stride,
                        layout.offset);

  glEnableVertexAttribArray(layout.index);

  vao->attrib_index++;
}

// @Shader =====================================================================================

R_Shader r_create_shader(const char *vert_src, const char *frag_src)
{
  u32 vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vert_src, NULL);
  glCompileShader(vert);

  u32 frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &frag_src, NULL);
  glCompileShader(frag);

  #ifdef DEBUG
  verify_shader(vert, GL_COMPILE_STATUS);
  verify_shader(frag, GL_COMPILE_STATUS);
  #endif

  u32 id = glCreateProgram();
  glAttachShader(id, frag);
  glAttachShader(id, vert);
  glLinkProgram(id);

  #ifdef DEBUG
  verify_shader(id, GL_LINK_STATUS);
  #endif

  glDeleteShader(vert);
  glDeleteShader(frag);

  return (R_Shader) {id};
}

inline
void r_set_uniform_1u(R_Shader *shader, const char *name, u32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1ui(loc, val);
}

inline
void r_set_uniform_1i(R_Shader *shader, const char *name, i32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1i(loc, val);
}

inline
void r_set_uniform_1f(R_Shader *shader, const char *name, f32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1f(loc, val);
}

inline
void r_set_uniform_2f(R_Shader *shader, const char *name, Vec2F val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform2f(loc, val.x, val.y);
}

inline
void r_set_uniform_3f(R_Shader *shader, const char *name, Vec3F val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform3f(loc, val.x, val.y, val.z);
}

inline
void r_set_uniform_4f(R_Shader *shader, const char *name, Vec4F val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform4f(loc, val.x, val.y, val.z, val.w);
}

inline
void r_set_uniform_3x3f(R_Shader *shader, const char *name, Mat3x3F val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniformMatrix3fv(loc, 1, FALSE, &val.e[0][0]);
}

#ifdef DEBUG
static
void verify_shader(u32 id, u32 type)
{
  i32 success;

  if (type == GL_LINK_STATUS)
  {
    glValidateProgram(id);
  }

  glGetShaderiv(id, type, &success);

  if (!success)
  {
    i32 length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char log[length];
    glGetShaderInfoLog(id, length, &length, log);

    if (type == GL_COMPILE_STATUS)
    {
      printf("[Error]: Failed to compile shader!\n");
    }
    else
    {
      printf("[Error]: Failed to link shaders!\n");
    }

    printf("%s", log);
  }
}
#endif

// @Texture ====================================================================================

R_Texture r_create_texture(const char *path)
{
  R_Texture tex;
  stbi_set_flip_vertically_on_load(TRUE);
  tex.data = stbi_load(path, &tex.width, &tex.height, &tex.channel_count, 4);

  glGenTextures(1, &tex.id);
  glBindTexture(GL_TEXTURE_2D, tex.id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(
               GL_TEXTURE_2D, 
               0, 
               GL_RGBA8, 
               tex.width, 
               tex.height, 
               0, 
               GL_RGBA, 
               GL_UNSIGNED_BYTE, 
               tex.data);

  stbi_image_free(tex.data);

  return tex;
}

inline
void r_bind_texture(R_Texture *texture, u32 slot)
{
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, texture->id);
}

// @Rendering ==================================================================================

R_Renderer r_create_renderer(u32 vertex_capacity, Arena *arena)
{
  u64 buffer_size = sizeof (R_Vertex) * vertex_capacity;
  R_Vertex *vertices = arena_alloc(arena, buffer_size);
  u32 *indices = arena_alloc(arena, buffer_size);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  R_VAO vao = r_create_vertex_array(3);
  u32 vbo = r_create_vertex_buffer(NULL, buffer_size, TRUE);
  u32 ibo = r_create_index_buffer(NULL, buffer_size, TRUE);
  r_push_vertex_attribute(&vao, GL_FLOAT, 4); // position
  r_push_vertex_attribute(&vao, GL_FLOAT, 4); // color
  r_push_vertex_attribute(&vao, GL_FLOAT, 4); // uv

  R_Texture *texture = &GLOBAL->resources.textures[0];

  return (R_Renderer)
  {
    .vertices = vertices,
    .vertex_count = 0,
    .vertex_capacity = vertex_capacity,
    .indices = indices,
    .index_count = 0,
    .vao = vao,
    .vbo = vbo,
    .ibo = ibo,
    .shader = (R_Shader) {0},
    .texture = texture,
  };
}

void r_push_vertex(R_Renderer *renderer, Vec4F pos, Vec4F tint, Vec4F uv)
{
  if (renderer->vertex_count == renderer->vertex_capacity)
  {
    r_flush(renderer);
  }

  renderer->vertices[renderer->vertex_count++] = (R_Vertex)
  {
    .position = pos,
    .tint = tint,
    .uv = uv,
  };
}

void r_push_quad_indices(R_Renderer *renderer)
{
  static const u32 layout[6] = 
  {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
  };

  u32 offset = renderer->vertex_count - 4;
  u32 index_count = renderer->index_count += 6;
  renderer->indices[index_count - 6] = layout[0] + offset;
  renderer->indices[index_count - 5] = layout[1] + offset;
  renderer->indices[index_count - 4] = layout[2] + offset;
  renderer->indices[index_count - 3] = layout[3] + offset;
  renderer->indices[index_count - 2] = layout[4] + offset;
  renderer->indices[index_count - 1] = layout[5] + offset;
}

void r_use_shader(R_Renderer *renderer, R_Shader shader)
{
  if (renderer->shader.id != shader.id)
  {
    r_flush(renderer);
    renderer->shader = shader;
  }
}

void r_flush(R_Renderer *renderer)
{
  if (renderer->vertex_count == 0) return;

  glUseProgram(renderer->shader.id);
  r_set_uniform_3x3f(&renderer->shader, "u_xform", m3x3f(1.0f));
  r_set_uniform_4f(&renderer->shader, "u_color", BLACK);
  r_set_uniform_1i(&renderer->shader, "u_tex", 0);

  glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
  r_update_vertex_buffer(renderer->vertices, sizeof (R_Vertex) * renderer->vertex_count, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ibo);
  r_update_index_buffer(renderer->indices, sizeof (u32) * renderer->index_count, 0);

  glBindVertexArray(renderer->vao.id);
  glDrawElements(GL_TRIANGLES, renderer->index_count, GL_UNSIGNED_INT, NULL);

  renderer->vertex_count = 0;
  renderer->index_count = 0;
}
