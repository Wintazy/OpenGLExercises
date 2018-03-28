#version 330
in vec3 vertexColor;
in vec2 TexCoord;

out vec4 fragColor;

uniform float customAlpha;
uniform sampler2D customTexture;

void main()
{
	fragColor = texture(customTexture, TexCoord) * vec4(vertexColor, customAlpha);
}