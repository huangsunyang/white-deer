#version 330 core

in vec4 f_lightSpacePos;
in vec2 f_tex;

out vec4 f_color;

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

    float shadow = 0.0;
    float bias = 0.005;
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

void main()
{
  f_color = vec4(vec3(0.2 + 0.8 * CalculateShadow()), 1.0);
}