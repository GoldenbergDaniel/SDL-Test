// @Vertex ///////////////////////////////////////////////////////////////////////////////
#version 410 core \n

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec4 a_tint;
layout (location = 2) in vec4 a_color;
layout (location = 3) in vec2 a_tex_coord;

out vec4 tint;
out vec4 flash_color;
out vec2 tex_coord;

uniform mat3 u_xform;

void main()
{
  gl_Position = vec4(a_pos * u_xform, 1.0);
  tint = a_tint;
  flash_color = a_color;
  tex_coord = a_tex_coord;
}

// @Fragment /////////////////////////////////////////////////////////////////////////////
#version 410 core \n

in vec4 tint;
in vec4 flash_color;
in vec2 tex_coord;

layout (location = 0) out vec4 frag_color;

uniform sampler2D u_tex;

void main()
{
  vec4 tex_color = texture(u_tex, tex_coord);
  frag_color = (tex_color * tint) + flash_color;
}
