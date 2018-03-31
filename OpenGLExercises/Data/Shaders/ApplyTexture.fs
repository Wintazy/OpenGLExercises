#version 330
in vec3 vertexColor;
in vec2 TexCoord;

out vec4 fragColor;

uniform sampler2D customTexture1;
uniform sampler2D customTexture2;

void main()
{
	fragColor = mix(texture(customTexture1, TexCoord), texture(customTexture2, TexCoord), texture(customTexture2, TexCoord).a);
}