#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float chosen;
uniform sampler2D stoneTexture;

void main()
{
     FragColor = texture(stoneTexture, TexCoord) * chosen; 
}