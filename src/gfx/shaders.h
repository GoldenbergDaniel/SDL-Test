const char *primitive_vert_src = "#version 410 core layout (location = 0) in vec3 a_pos; layout (location = 1) in vec3 a_color; out vec3 color; uniform mat3 u_xform; void main() {   gl_Position = vec4(a_pos * u_xform, 1.0);   color = a_color; } ";
const char *primitive_frag_src = "#version 410 core in vec3 color; layout (location = 0) out vec4 frag_color; uniform vec4 u_color = vec4(0); void main() {   vec4 final_color = u_color + vec4(color, 1.0);   frag_color = final_color; } ";

const char *sprite_vert_src = "#version 410 core layout (location = 0) in vec3 a_pos; layout (location = 1) in vec3 a_color; layout (location = 2) in vec3 a_tex_coord; out vec3 color; out vec2 tex_coord; uniform mat3 u_xform; void main() {   gl_Position = vec4(a_pos * u_xform, 1.0);   color = a_color;   tex_coord = vec2(a_tex_coord.x, a_tex_coord.y); } ";
const char *sprite_frag_src = "#version 410 core in vec3 color; in vec2 tex_coord; layout (location = 0) out vec4 frag_color; uniform vec4 u_color = vec4(0); uniform sampler2D u_tex; void main() {   vec4 tex_color = texture(u_tex, tex_coord) * (u_color + vec4(color, 1.0));   frag_color = tex_color; } ";

