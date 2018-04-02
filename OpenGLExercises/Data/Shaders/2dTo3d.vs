#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormVec;

out vec3 NormalVec;
out vec3 FragPos;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main()
{
	gl_Position = projectionMat * viewMat * modelMat * vec4(aPos, 1.0);
	FragPos = vec3(modelMat * vec4(aPos, 1.0));
	NormalVec = mat3(transpose(inverse(modelMat))) * aNormVec;
}