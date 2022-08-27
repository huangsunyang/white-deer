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
  int type;
  vec3 pos;
  vec3 dir;
  vec3 color;

  // attenuation
  float constant;
  float linear;
  float quadratic;

  // spot light range
  float cutoff;
  float outerCutoff;
};

uniform Material u_material;
uniform Light u_light;

void main()
{
  // normalize
  vec3 normal = normalize(f_normal);
  vec3 lightDir = normalize(u_light.dir);
  float attenuation = 1.0;
  float cutoffIntensity = 1.0;

  if (u_light.type > 0) {
    // point light has attenuation
    lightDir = normalize(f_worldPos - u_light.pos);
    float distance = length(f_worldPos - u_light.pos);
    attenuation = u_light.constant + u_light.linear * distance + u_light.quadratic * distance * distance;
    attenuation = 1.0 / attenuation;

    if (u_light.type == 2) {
      // spot light
      float theta = dot(lightDir, normalize(u_light.dir));
      float outerCutoff = min(u_light.cutoff - 0.001, u_light.outerCutoff);
      float delta = u_light.cutoff - outerCutoff;
      cutoffIntensity = clamp((theta - outerCutoff) / delta, 0.0, 1.0);
    }
  }

  // ambient
  vec3 ambient = u_material.ambient * u_light.color;

  // diffuse
  float diff = max(dot(normal, -lightDir), 0.0);
  vec3 diffuse = u_material.diffuse * diff * u_light.color;

  // specular
  vec3 viewDir = normalize(u_viewPos - f_worldPos);
  vec3 lightReflect = reflect(lightDir, normal);
  float spec = pow(max(dot(viewDir, lightReflect), 0.0), u_material.shininess);
  vec3 specular = u_material.specular * spec * u_light.color;

  // final color
  vec4 texColor = texture(u_texture, f_tex);
  f_color = vec4(ambient + (diffuse + specular) * cutoffIntensity, 1.0) * attenuation * texColor;
}