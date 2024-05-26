// @Vertex ///////////////////////////////////////////////////////////////////////////////
#version 410 core \n

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec4 a_tint;

out vec4 color;

uniform mat3 u_projection;

void main()
{
  gl_Position = vec4(a_pos * u_projection, 1.0);
  color = a_tint;
}

// @Fragment /////////////////////////////////////////////////////////////////////////////
#version 410 core \n

in vec4 color;

layout (location = 0) out vec4 frag_color;

void main()
{
  frag_color = color;
}
