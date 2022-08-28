#version 330 core

in vec2 f_tex;

out vec4 f_color;

uniform sampler2D u_screen;

void main() {
  f_color = texture(u_screen, f_tex);
  float gray = 0.2126 * f_color.r + 0.7152 * f_color.g + 0.0722 * f_color.b;
  f_color = vec4(gray, gray, gray, 1.0);
}