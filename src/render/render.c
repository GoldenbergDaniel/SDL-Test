#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#elif defined(_WIN32)
#include "glad/glad.h"
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb/stb_image.h"

#include "../base/base_inc.h"
#include "render.h"

#ifndef RELEASE
static void verify_shader(u32 id, u32 type);
#endif

inline
void r_set_viewport(i32 x, i32 y, i32 w, i32 h)
{
  glViewport(x, y, w, h);
}

// @Buffer ///////////////////////////////////////////////////////////////////////////////

u32 r_create_vertex_buffer(void *data, u32 size, bool dynamic)
{
  u32 id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  GLenum draw_type = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
  glBufferData(GL_ARRAY_BUFFER, size, data, draw_type);

  return id;
}

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

void r_update_index_buffer(void *data, u32 size, u32 offset)
{
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

// @RAO //////////////////////////////////////////////////////////////////////////////////

R_VAO r_create_vertex_array(u8 stride)
{
  u32 id;
  glGenVertexArrays(1, &id);
  glBindVertexArray(id);

  return (R_VAO) 
  {
    .id = id, 
    .stride = stride, 
    .attrib_index = 0,
  };
}

void r_push_vertex_attribute(R_VAO *vao, u32 count)
{
  glVertexAttribPointer(vao->attrib_index,
                        count,
                        GL_FLOAT,
                        FALSE,
                        sizeof (f32) * vao->stride,
                        (void *) (sizeof (f32) * vao->offset));

  glEnableVertexAttribArray(vao->attrib_index);

  vao->offset += count;
  vao->attrib_index++;
}

// @Shader ///////////////////////////////////////////////////////////////////////////////

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

  u32 program = glCreateProgram();
  glAttachShader(program, frag);
  glAttachShader(program, vert);
  glLinkProgram(program);

  #ifdef DEBUG
  verify_shader(program, GL_LINK_STATUS);
  #endif

  glDeleteShader(vert);
  glDeleteShader(frag);

  i16 u_xform = glGetUniformLocation(program, "u_projection");
  i16 u_tex = glGetUniformLocation(program, "u_tex");

  return (R_Shader) {
    .id = program,
    .u_xform = u_xform,
    .u_tex = u_tex,
  };
}

inline
void r_set_uniform_1u(R_Shader *shader, i32 loc, u32 val)
{
  glUniform1ui(loc, val);
}

inline
void r_set_uniform_1i(R_Shader *shader, i32 loc, i32 val)
{
  glUniform1i(loc, val);
}

inline
void r_set_uniform_1f(R_Shader *shader, i32 loc, f32 val)
{
  glUniform1f(loc, val);
}

inline
void r_set_uniform_2f(R_Shader *shader, i32 loc, Vec2F val)
{
  glUniform2f(loc, val.x, val.y);
}

inline
void r_set_uniform_3f(R_Shader *shader, i32 loc, Vec3F val)
{
  glUniform3f(loc, val.x, val.y, val.z);
}

inline
void r_set_uniform_4f(R_Shader *shader, i32 loc, Vec4F val)
{
  glUniform4f(loc, val.x, val.y, val.z, val.w);
}

inline
void r_set_uniform_3x3f(R_Shader *shader, i32 loc, Mat3x3F val)
{
  glUniformMatrix3fv(loc, 1, FALSE, &val.e[0][0]);
}

// @Texture //////////////////////////////////////////////////////////////////////////////

R_Texture r_create_texture(String path)
{
  static u8 tex_slot = 0;
  R_Texture tex = {0};
  tex.slot = tex_slot++;

  stbi_set_flip_vertically_on_load(TRUE);
  u8 *data = stbi_load(path.data, &tex.width, &tex.height, NULL, 4);

  glGenTextures(1, &tex.id);
  glBindTexture(GL_TEXTURE_2D, tex.id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 
               0, 
               GL_RGBA8, 
               tex.width, 
               tex.height, 
               0, 
               GL_RGBA, 
               GL_UNSIGNED_BYTE, 
               data);

  stbi_image_free(data);

  return tex;
}

// @Rendering ////////////////////////////////////////////////////////////////////////////

R_Renderer r_create_renderer(u32 vertex_capacity, u16 w, u16 h, Arena *arena)
{
  u64 vbo_size = sizeof (R_Vertex) * vertex_capacity;
  R_Vertex *vertices = arena_push(arena, vbo_size);
  u64 ibo_size = (u64) (sizeof (u32) * vertex_capacity * 1.5f) + 1;
  u32 *indices = arena_push(arena, ibo_size);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  R_VAO vao = r_create_vertex_array(sizeof (R_Vertex) / sizeof (f32));
  u32 vbo = r_create_vertex_buffer(NULL, vbo_size, TRUE);
  u32 ibo = r_create_index_buffer(NULL, ibo_size, TRUE);
  r_push_vertex_attribute(&vao, 3); // position
  r_push_vertex_attribute(&vao, 4); // tint
  r_push_vertex_attribute(&vao, 2); // uv
  r_push_vertex_attribute(&vao, 1); // flash

  Mat3x3F projection = orthographic_3x3f(0.0f, w, h, 0.0f);

  return (R_Renderer) {
    .vertices = vertices,
    .vertex_count = 0,
    .vertex_capacity = vertex_capacity,
    .indices = indices,
    .index_count = 0,
    .vao = vao,
    .vbo = vbo,
    .ibo = ibo,
    .shader = R_NIL_SHADER,
    .texture = R_NIL_TEXTURE,
    .projection = projection,
  };
}

void r_push_vertex(R_Renderer *renderer, Vec3F pos, Vec4F tint, Vec4F color, Vec2F uv)
{
  if (renderer->vertex_count == renderer->vertex_capacity)
  {
    r_flush(renderer);
  }

  renderer->vertices[renderer->vertex_count++] = (R_Vertex) {
    .pos = pos,
    .tint = tint,
    .uv = uv,
    .flash = color.r,
  };
}

void r_push_quad_indices(R_Renderer *renderer)
{
  static u32 layout[6] = {
    0, 1, 3,
    1, 2, 3,
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

void r_use_shader(R_Renderer *renderer, R_Shader *shader)
{
  if (renderer->shader->id != shader->id)
  {
    r_flush(renderer);

    renderer->shader = shader;
    glUseProgram(shader->id);
  }
}

void r_use_texture(R_Renderer *renderer, R_Texture *texture)
{
  if (renderer->texture->id != texture->id)
  {
    r_flush(renderer);

    renderer->texture = texture;
    glActiveTexture(GL_TEXTURE0 + texture->slot);
    glBindTexture(GL_TEXTURE_2D, texture->id);
  }
}

void r_flush(R_Renderer *renderer)
{
  if (renderer->vertex_count == 0) return;

  R_Shader *shader = renderer->shader;
  r_set_uniform_3x3f(shader, shader->u_xform, renderer->projection);
  r_set_uniform_1i(shader, shader->u_tex, renderer->texture->slot);
  
  glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
  r_update_vertex_buffer(renderer->vertices, sizeof (R_Vertex) * renderer->vertex_count, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ibo);
  r_update_index_buffer(renderer->indices, sizeof (u32) * renderer->index_count, 0);

  glBindVertexArray(renderer->vao.id);
  glDrawElements(GL_TRIANGLES, renderer->index_count, GL_UNSIGNED_INT, NULL);

  renderer->vertex_count = 0;
  renderer->index_count = 0;
}

// @Debug ////////////////////////////////////////////////////////////////////////////////

#ifndef RELEASE
static
void verify_shader(u32 id, u32 type)
{
  if (type == GL_LINK_STATUS)
  {
    glValidateProgram(id);
  }

  i32 success = TRUE;
  glGetShaderiv(id, type, &success);

  if (!success)
  {
    i32 length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char log[1000];
    glGetShaderInfoLog(id, length, &length, log);

    if (type == GL_COMPILE_STATUS)
    {
      printf("[ERROR]: Failed to compile shader!\n");
    }
    else
    {
      printf("[ERROR]: Failed to link shaders!\n");
    }

    printf("%s", log);
  }
}
#endif
