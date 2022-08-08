#version 330 core
layout (location = 0) in vec3 pos;

void main()
{
    gl_Position = vec4(pos.x / 3, pos.y / 3, pos.z / 2, 1.0);
}