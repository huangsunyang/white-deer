#version 330 core

in vec3 f_worldPos;
in vec3 f_normal;
in vec2 f_tex;

out vec4 f_color;

uniform vec3 u_viewPos;
uniform sampler2D u_texture;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 dir;
  vec3 color;
};

uniform Material u_material;
uniform Light u_light;

void main()
{
  vec3 normal = normalize(f_normal);
  vec3 ambient = u_material.ambient * u_light.color;

  // diffuse
  float diff = max(dot(normal, u_light.dir), 0.0);
  vec3 diffuse = u_material.diffuse * diff * u_light.color;

  // specular
  vec3 viewDir = normalize(u_viewPos - f_worldPos);
  vec3 lightReflect = reflect(-u_light.dir, normal);
  float spec = pow(max(dot(viewDir, lightReflect), 0.0), u_material.shininess);
  vec3 specular = u_material.specular * spec * u_light.color;

  // final color
  vec4 texColor = texture(u_texture, f_tex);
  f_color = vec4(diffuse + ambient + specular, 1.0) * texColor;
}