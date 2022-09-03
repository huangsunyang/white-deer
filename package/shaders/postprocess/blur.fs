#version 330 core

in vec2 f_tex;

out vec4 f_color;

uniform sampler2D u_screen;

void main() {
  vec2 texelSize = 1.0 / textureSize(u_screen, 0);
  texelSize = 1.0 / 500 * vec2(1, 1);
  float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16
  );

  vec4 color = vec4(0.0);
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      vec2 tex = f_tex + texelSize * vec2(i, j);
      int index = (i + 1) * 3 + j + 1;
      color += texture(u_screen, tex) * kernel[index];
    }
  }
  f_color = vec4(color.xyz, 1.0);
}