#version 330 core

in vec3 f_normal;
in vec2 f_tex;

out vec4 f_color;

uniform vec3 u_lightDir;
uniform vec3 u_lightColor;
uniform sampler2D u_texture;

void main()
{
  vec3 normal = normalize(f_normal);
  vec3 ambient = 0.1 * u_lightColor;

  float diff = max(dot(normal, u_lightDir), 0.0);
  vec3 diffuse = diff * u_lightColor + vec3(0.1);
  vec4 texColor = texture(u_texture, f_tex);
  f_color = vec4(diffuse + ambient, 1.0) * texColor;
}