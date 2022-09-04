#version 330 core

in vec2 f_tex;

out vec4 f_color;

uniform sampler2D u_screen;

void main() {
  float exposure = 2.0;
  vec3 color = texture(u_screen, f_tex).rgb;
  color = vec3(1.0) - exp(-color * exposure);
  f_color = vec4(color , 1.0);
}