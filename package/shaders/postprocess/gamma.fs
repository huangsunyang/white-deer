#version 330 core

in vec2 f_tex;

out vec4 f_color;

uniform sampler2D u_screen;

void main() {
  f_color = texture(u_screen, f_tex);
  f_color.rgb = pow(f_color.rgb, vec3(1.0 / 2.2));
}