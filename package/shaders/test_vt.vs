#version 330 core
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 v_tex;

out vec2 f_tex;
out vec4 f_lightSpacePos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(v_pos, 1.0);
    f_tex = v_tex;
    f_lightSpacePos = lightMatrix * model * vec4(v_pos, 1.0);
}