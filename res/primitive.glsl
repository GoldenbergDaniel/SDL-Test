// @Vertex =====================================================================================
#version 410 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec4 a_color;

out vec4 color;

uniform mat3 u_xform;

void main()
{
  gl_Position = vec4(a_pos * u_xform, 1.0);
  color = a_color;
}

// @Fragment ===================================================================================
#version 410 core

in vec4 color;
layout (location = 0) out vec4 frag_color;

uniform vec4 u_color = vec4(0);

void main()
{
  vec4 final_color = u_color + color;
  frag_color = final_color;
}
