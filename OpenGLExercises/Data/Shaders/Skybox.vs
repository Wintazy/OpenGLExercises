#version 330
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projectionMat;
uniform mat4 viewMat;

void main()
{
    TexCoords = aPos;
    gl_Position = projectionMat * viewMat * vec4(aPos, 1.0);
}