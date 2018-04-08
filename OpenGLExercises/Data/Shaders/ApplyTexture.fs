#version 330
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D customTexture1;

void main()
{
	FragColor =  texture(customTexture1, TexCoords);
}