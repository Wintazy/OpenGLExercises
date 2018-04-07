#version 330

layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 projectionMat;

void main()
{
	gl_Position = projectionMat * vec4(vertex.xy, 0.0, 1.0);
	TexCoords = vertex.zw;
}