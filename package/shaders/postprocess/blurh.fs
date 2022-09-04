#version 330 core

in vec2 f_tex;

out vec4 f_color;

uniform bool horizontal = true;
uniform sampler2D u_screen;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main() {
  vec3 color = texture(u_screen, f_tex).rgb * weight[0];
  vec2 pixel = 1.0 / textureSize(u_screen, 0);
  pixel = horizontal ? vec2(pixel.x, 0) : vec2(0, pixel.y);

  for (int i = 1; i < 5; i++) {
    color += texture(u_screen, f_tex + pixel).rgb * weight[i];
    color += texture(u_screen, f_tex - pixel).rgb * weight[i];
  }
  f_color = vec4(color, 1.0);
}