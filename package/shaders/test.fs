#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D myTexture;

void main()
{
  FragColor = vec4(1, 0.5, 0.5, 1);
//   FragColor = texture(myTexture, TexCoord);
}