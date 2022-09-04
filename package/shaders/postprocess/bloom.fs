#version 330 core

in vec2 f_tex;

out vec4 f_color;

uniform sampler2D u_screen0;
uniform sampler2D u_screen1;

void main() {
  vec3 color = texture(u_screen0, f_tex).rgb + texture(u_screen1, f_tex).rgb;
  f_color = vec4(color, 1.0);
}