#version 330 core

in vec3 f_normal;
in vec2 f_tex;

out vec4 f_color;

uniform sampler2D u_texture;

void main()
{
  vec3 norm = normalize(f_normal);
  vec3 lightDir = normalize(vec3(1, 1, 1));
  vec3 lightColor = vec3(1,1,1);
  vec3 ambient = 0.1 * lightColor;

  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor + vec3(0.1);
  vec4 objColor = texture(u_texture, f_tex);
  f_color = vec4(diffuse + ambient, 1.0) * objColor;
}