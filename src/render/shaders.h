#pragma once

const char *PRIMITIVE_VERT_SRC = "#version 410 core \n layout (location = 0) in vec3 a_pos; layout (location = 1) in vec4 a_tint; out vec4 color; uniform mat3 u_projection; void main() {   gl_Position = vec4(a_pos * u_projection, 1.0);   color = a_tint; } ";
const char *PRIMITIVE_FRAG_SRC = "#version 410 core \n in vec4 color; layout (location = 0) out vec4 frag_color; void main() {   frag_color = color; } ";
const char *SPRITE_VERT_SRC = "#version 410 core \n layout (location = 0) in vec3 a_pos; layout (location = 1) in vec4 a_tint; layout (location = 2) in vec2 a_tex_coord; layout (location = 3) in float a_flash; out vec4 tint; out float flash; out vec2 tex_coord; uniform mat3 u_projection; void main() {   gl_Position = vec4(a_pos * u_projection, 1.0);   tint = a_tint;   flash = a_flash;   tex_coord = a_tex_coord; } ";
const char *SPRITE_FRAG_SRC = "#version 410 core \n in vec4 tint; in float flash; in vec2 tex_coord; layout (location = 0) out vec4 frag_color; uniform sampler2D u_tex; void main() {   vec4 tex_color = texture(u_tex, tex_coord);   frag_color = (tex_color * tint) + vec4(flash, flash, flash, 0); } ";
