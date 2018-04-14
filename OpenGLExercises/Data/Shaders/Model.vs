#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormVec;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 NormVec;

out VS_OUT {
    vec2 texCoords;
} vs_out;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main()
{
	FragPos = aPos;
	gl_Position = projectionMat * viewMat * modelMat * vec4(aPos, 1.0);
	TexCoords = aTexCoords;
	NormVec = aNormVec;
	vs_out.texCoords = aTexCoords;
}