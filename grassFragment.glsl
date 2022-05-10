#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float chosen;
uniform sampler2D grassTexture;

void main()
{
      FragColor = texture(grassTexture, TexCoord) * chosen;
}