#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float chosen;
uniform sampler2D sandTexture;

void main()
{
     FragColor = texture(sandTexture, TexCoord) * chosen; 
}