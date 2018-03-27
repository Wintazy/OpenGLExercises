#version 330
in vec3 vertexColor;
out vec4 fragColor;
uniform float customAlpha;

void main()
{
	fragColor = vec4(vertexColor, customAlpha);
}