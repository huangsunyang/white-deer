// vs
#version 330 core
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_tex;

out vec3 f_worldPos;
out vec2 f_tex;
out vec3 f_normal;
out vec4 f_lightSpacePos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 lightMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(v_pos, 1.0);
    f_worldPos = vec3(model * vec4(v_pos, 1.0));
    f_tex = v_tex;
    f_normal = v_normal;
    f_lightSpacePos = lightMatrix * model * vec4(v_pos, 1.0);
}

// fs
#version 330 core

in vec3 f_worldPos;
in vec3 f_normal;
in vec2 f_tex;
in vec4 f_lightSpacePos;

out vec4 f_color;

struct Material {
  // infact, these are about color
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  // specular
  float shininess;

  // Fresnel
  float fresnel;
};

struct Light {
  int type;
  vec3 pos;
  vec3 dir;
  vec3 color;
  float strength;

  // attenuation
  float constant;
  float linear;
  float quadratic;

  // spot light range
  float cutoff;
  float outerCutoff;
};

uniform vec3 u_viewPos;
uniform sampler2D u_texture;
uniform samplerCube u_skybox;

uniform Material u_material;
uniform Light u_lights[16];
uniform int u_lightNum;

uniform int u_hasShadowMap;
uniform int u_pcfCount;
uniform sampler2D u_shadowMap;

float CalculateShadow() {
  if (u_hasShadowMap > 0) {
    vec3 projCoords = f_lightSpacePos.xyz / f_lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;

    // farther than far plane
    if (projCoords.z > 1.0) {
      return 1.0;
    }

    float bias = max(0.005 * (1 - dot(f_normal, normalize(-f_lightSpacePos.xyz))), 0.001);
    float shadow = 0.0;
    int num = abs(u_pcfCount);
    vec2 texelSize = 1.0 / textureSize(u_shadowMap, 0);
    for (int x = -num; x <= num; x++) {
      for (int y = -num; y <= num; y++) {
        float closestDepth = texture(u_shadowMap, projCoords.xy).r;
        float pcfDepth = texture(u_shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
        shadow += projCoords.z - bias > pcfDepth ? 0.0 : 1.0;
      }
    }
    return shadow / (2 * num + 1) / (2 * num + 1);
  } else {
    return 1.0;
  }
}

vec3 CalculateLight(int i) {
  Light u_light = u_lights[i];

  // normalize
  vec3 normal = normalize(f_normal);
  vec3 lightDir = normalize(u_light.dir);
  float attenuation = 1.0;
  float cutoffIntensity = 1.0;

  if (u_light.type > 0) {
    // point light and spot light has attenuation
    lightDir = normalize(f_worldPos - u_light.pos);
    float distance = length(f_worldPos - u_light.pos);
    attenuation = u_light.constant + u_light.linear * distance + u_light.quadratic * distance * distance;
    attenuation = 1.0 / attenuation;

    if (u_light.type == 2) {
      // spot light has a spot range called cutoff
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
  float shadow = CalculateShadow();
  return (ambient + (diffuse + specular) * cutoffIntensity * shadow * attenuation) * u_light.strength;
}

void main() {
  vec3 lightColor = vec3(0, 0, 0);
  for (int i = 0; i < u_lightNum; i++) {
    lightColor += CalculateLight(i);
  }

  // reflect env color
  vec3 normal = normalize(f_normal);
  vec3 viewDir = normalize(u_viewPos - f_worldPos);
  vec3 viewReflect = reflect(-viewDir, normal);
  vec4 envColor = texture(u_skybox, viewReflect);

  // Fresnel
  float viewNormalDot = dot(viewDir, normal);
  vec4 texColor = texture(u_texture, f_tex);
  float fresnel = max(u_material.fresnel, 0.0);
  vec4 finalColor = mix(envColor, texColor, pow(viewNormalDot, fresnel));

  f_color = vec4(lightColor, 1.0) * finalColor;
}