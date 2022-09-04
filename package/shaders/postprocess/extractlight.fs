#version 330 core

in vec2 f_tex;

out vec4 f_color;

uniform sampler2D u_screen;

void main() {
  f_color = texture(u_screen, f_tex);
  float brightness = dot(f_color.rgb, vec3(0.2126, 0.7152, 0.0722));
  if(brightness <= 1.0) {
    f_color = vec4(0.0, 0.0, 0.0, 1.0);
  }
}