#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 vertexColor;

uniform mat4 transformMat;

void main()
{
	gl_Position = transformMat * vec4(aPos, 1.0);
	vertexColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}