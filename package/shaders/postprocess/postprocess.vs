#version 330 core
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 v_tex;

out vec2 f_tex;


void main() {
  f_tex = v_tex;
  gl_Position = vec4(v_pos, 1.0);
}