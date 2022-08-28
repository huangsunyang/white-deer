#version 330 core

in vec2 f_tex;

out vec4 f_color;

uniform sampler2D u_screen;

// todo: how do i know a pixel size?
const float offset = 1.0 / 100.0;

void main() {
  float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16
  );

  vec4 color = vec4(0.0);
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      vec2 tex = f_tex;
      tex.x += i * offset;
      tex.y += j * offset;
      int index = (i + 1) * 3 + j + 1;
      color += texture(u_screen, tex) * kernel[index];
    }
  }
  f_color = vec4(color.xyz, 1.0);
}