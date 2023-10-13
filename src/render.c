#include <SDL2/SDL.h>
#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb/stb_image.h"

#include "base_common.h"
#include "base_math.h"
#include "render.h"

typedef R_Shader Shader;
typedef R_Object Object;
typedef R_Texture2D Texture2D;
typedef R_VertexLayout VertexLayout;

static void r_verify_shader(u32 id, u32 type);

bool _r_check_error(void)
{
  bool error = FALSE;

  for (u32 err = -1; (err = glGetError());)
  {
    printf("[OpenGL Error]: %u\n", err);
    error = TRUE;
  }

  return error;
}

void _r_clear_error(void)
{
  while (glGetError() != GL_NO_ERROR);
}

// @Buffer ==================================================================================

Object r_create_vertex_buffer(void *data, u32 size)
{
  u32 id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

  return (Object) {id, 0, 0};
}

inline
void r_bind_vertex_buffer(Object *buffer)
{
  glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
}

inline
void r_unbind_vertex_buffer(void)
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Object r_create_index_buffer(void *data, u32 size)
{
  u32 id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

  return (Object) {id, 0, 0};
}

inline
void r_bind_index_buffer(Object *buffer)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->id);
}

inline
void r_unbind_index_buffer(void)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// @VertexArray =============================================================================

Object r_create_vertex_array(u8 attrib_count)
{
  u32 id;
  R_ASSERT(glGenVertexArrays(1, &id));
  R_ASSERT(glBindVertexArray(id));

  return (Object) {id, attrib_count, 0};
}

inline
void r_bind_vertex_array(Object *vertex_array)
{
  R_ASSERT(glBindVertexArray(vertex_array->id));
}

inline
void r_unbind_vertex_array(void)
{
  glBindVertexArray(0);
}

void r_create_vertex_layout(Object *vertex_array, u32 data_type, u32 count)
{
  u8 type_size;
  switch (data_type)
  {
    case GL_BYTE:  type_size = sizeof (i8);  break;
    case GL_SHORT: type_size = sizeof (i16); break;
    case GL_INT:   type_size = sizeof (i32); break;
    case GL_FLOAT: type_size = sizeof (f32); break;
    default: ASSERT(FALSE);
  }

  VertexLayout layout = 
  {
    .index = vertex_array->attrib_index,
    .count = count,
    .data_type = data_type,
    .normalized = FALSE,
    .stride = count * vertex_array->attrib_count * type_size,
    .first = (void *) (u64) (vertex_array->attrib_index * count * type_size)
  };

  vertex_array->attrib_index++;

  R_ASSERT(glVertexAttribPointer(
                                 layout.index,
                                 layout.count,
                                 layout.data_type,
                                 layout.normalized,
                                 layout.stride,
                                 layout.first));

  R_ASSERT(glEnableVertexAttribArray(layout.index));
}

// @Shader ==================================================================================

Shader r_create_shader(const i8 *vert_src, const i8 *frag_src)
{
  u32 vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vert_src, NULL);
  glCompileShader(vert);

  u32 frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &frag_src, NULL);
  glCompileShader(frag);

  #ifdef DEBUG
  r_verify_shader(vert, GL_COMPILE_STATUS);
  r_verify_shader(frag, GL_COMPILE_STATUS);
  #endif

  u32 id = glCreateProgram();
  glAttachShader(id, frag);
  glAttachShader(id, vert);
  glLinkProgram(id);

  #ifdef DEBUG
  r_verify_shader(id, GL_LINK_STATUS);
  #endif

  glDeleteShader(vert);
  glDeleteShader(frag);

  return (Shader) {id};
}

inline
void r_bind_shader(Shader *shader)
{
  R_ASSERT(glUseProgram(shader->id));
}

inline
void r_unbind_shader(void)
{
  glUseProgram(0);
}

i32 r_set_uniform_1u(Shader *shader, i8 *name, u32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1ui(loc, val);
  
  return loc;
}

i32 r_set_uniform_1(Shader *shader, i8 *name, i32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1i(loc, val);
  return loc;
}

i32 r_set_uniform_1f(Shader *shader, i8 *name, f32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1f(loc, val);
  
  return loc;
}

i32 r_set_uniform_2f(Shader *shader, i8 *name, Vec2F vec)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform2f(loc, vec.x, vec.y);

  return loc;
}

i32 r_set_uniform_3f(Shader *shader, i8 *name, Vec3F vec)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform3f(loc, vec.x, vec.y, vec.z);
  
  return loc;
}

i32 r_set_uniform_4f(Shader *shader, i8 *name, Vec4F vec)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
  
  return loc;
}

i32 r_set_uniform_4x4f(Shader *shader, i8 *name, Mat4x4F mat)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniformMatrix4fv(loc, 1, FALSE, &mat.elements[0][0]);
  
  return loc;
}

i32 r_set_uniform_3x3f(Shader *shader, i8 *name, Mat3x3F mat)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniformMatrix3fv(loc, 1, FALSE, &mat.elements[0][0]);
  
  return loc;
}

static
void r_verify_shader(u32 id, u32 type)
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
    i8 log[length];
    glGetShaderInfoLog(id, length, &length, log);

    if (type == GL_COMPILE_STATUS)
    {
      printf("[GLObject Error]: Failed to compile shader!\n");
    }
    else
    {
      printf("[GLObject Error]: Failed to link shaders!\n");
    }

    printf("%s", log);
  }
}

// @Texture2D ===============================================================================

Texture2D r_load_texture2d(const i8 *path)
{
  Texture2D tex;
  glGenTextures(1, &tex.id);
  tex.data = stbi_load(path, &tex.width, &tex.height, &tex.num_channels, 0);

  glTexImage2D(
              GL_TEXTURE_2D, 
              0, 
              GL_RGB, 
              tex.width, 
              tex.height, 
              0, 
              GL_RGB, 
              GL_UNSIGNED_BYTE, 
              tex.data);

  glGenerateMipmap(GL_TEXTURE_2D);

  return tex;
}

inline
void r_bind_texture2d(Texture2D *texture)
{
  glBindTexture(GL_TEXTURE_2D, texture->id);
}

inline
void r_unbind_texture2d(void)
{
  glBindTexture(GL_TEXTURE_2D, 0);
}

// @Draw ====================================================================================

void r_clear(Vec4F color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void r_draw(Object *vertex_array, Shader *shader)
{
  r_bind_shader(shader);
  r_bind_vertex_array(vertex_array);
  R_ASSERT(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL));
}
