#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main()
{
    Normal = mat3(transpose(inverse(modelMat))) * aNormal;
    Position = vec3(modelMat * vec4(aPos, 1.0));
    gl_Position = projectionMat * viewMat * modelMat * vec4(aPos, 1.0);
}