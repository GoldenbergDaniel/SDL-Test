#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb/stb_image.h"

#include "base/base_common.h"
#include "base/base_math.h"
#include "gl_render.h"

typedef R_Shader Shader;
typedef R_Texture Texture;
typedef R_VAO VAO;

static void verify_shader(u32 id, u32 type);

bool _r_gl_check_error(void)
{
  bool result = FALSE;

  for (u32 err = -1; (err = glGetError());)
  {
    printf("[OpenGL Error]: %u\n", err);
    result = TRUE;
  }

  return result;
}

void _r_gl_clear_error(void)
{
  while (glGetError() != GL_NO_ERROR);
}

// @Buffer =====================================================================================

u32 r_gl_create_vertex_buffer(void *data, u32 size)
{
  u32 id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

  return id;
}

inline
void r_gl_bind_vertex_buffer(u32 id)
{
  glBindBuffer(GL_ARRAY_BUFFER, id);
}

inline
void r_gl_unbind_vertex_buffer(void)
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

u32 r_gl_create_index_buffer(void *data, u32 size)
{
  u32 id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

  return id;
}

inline
void r_gl_bind_index_buffer(u32 id)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

inline
void r_gl_unbind_index_buffer(void)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// @VAO ========================================================================================

VAO r_gl_create_vertex_array(u8 attrib_count)
{
  u32 id;
  GL_ASSERT(glGenVertexArrays(1, &id));
  GL_ASSERT(glBindVertexArray(id));

  return (VAO) {id, attrib_count, 0};
}

inline
void r_gl_bind_vertex_array(VAO *vao)
{
  GL_ASSERT(glBindVertexArray(vao->id));
}

inline
void r_gl_unbind_vertex_array(void)
{
  glBindVertexArray(0);
}

void r_gl_add_vertex_attribute(VAO *vao, GLenum data_type, u32 count)
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
    case GL_BYTE:  type_size = sizeof (i8);  break;
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
    .stride = count * vao->attrib_count * type_size,
    .offset = (void *) (u64) (vao->attrib_index * count * type_size)
  };

  vao->attrib_index++;

  GL_ASSERT(glVertexAttribPointer(
                                  layout.index,
                                  layout.count,
                                  layout.data_type,
                                  layout.normalized,
                                  layout.stride,
                                  layout.offset));

  GL_ASSERT(glEnableVertexAttribArray(layout.index));
}

// @Shader =====================================================================================

Shader r_gl_create_shader(const i8 *vert_src, const i8 *frag_src)
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

  return (Shader) {id};
}

inline
void r_gl_bind_shader(Shader *shader)
{
  GL_ASSERT(glUseProgram(shader->id));
}

inline
void r_gl_unbind_shader(void)
{
  glUseProgram(0);
}

inline
i32 r_gl_set_uniform_1u(Shader *shader, i8 *name, u32 v)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1ui(loc, v);
  
  return loc;
}

inline
i32 r_gl_set_uniform_1i(Shader *shader, i8 *name, i32 v)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1i(loc, v);

  return loc;
}

inline
i32 r_gl_set_uniform_1f(Shader *shader, i8 *name, f32 v)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1f(loc, v);
  
  return loc;
}

inline
i32 r_gl_set_uniform_2f(Shader *shader, i8 *name, Vec2F v)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform2f(loc, v.x, v.y);

  return loc;
}

inline
i32 r_gl_set_uniform_3f(Shader *shader, i8 *name, Vec3F v)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform3f(loc, v.x, v.y, v.z);
  
  return loc;
}

inline
i32 r_gl_set_uniform_4f(Shader *shader, i8 *name, Vec4F v)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform4f(loc, v.x, v.y, v.z, v.w);
  
  return loc;
}

inline
i32 r_gl_set_uniform_4x4f(Shader *shader, i8 *name, Mat4x4F m)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniformMatrix4fv(loc, 1, FALSE, &m.elements[0][0]);
  
  return loc;
}

inline
i32 r_gl_set_uniform_3x3f(Shader *shader, i8 *name, Mat3x3F m)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniformMatrix3fv(loc, 1, FALSE, &m.elements[0][0]);
  
  return loc;
}

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
    i8 log[length];
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

// @Texture ====================================================================================

Texture r_gl_create_texture(const i8 *path)
{
  Texture tex;
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

  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(tex.data);

  return tex;
}

inline
void r_gl_bind_texture(Texture *texture, u32 slot)
{
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, texture->id);
}

inline
void r_gl_unbind_texture(void)
{
  glBindTexture(GL_TEXTURE_2D, 0);
}

// @Draw =======================================================================================

inline
void r_gl_clear_screen(Vec4F color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

inline
void r_gl_draw_triangles(u32 vertex_count)
{
  GL_ASSERT(glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, NULL));
}
