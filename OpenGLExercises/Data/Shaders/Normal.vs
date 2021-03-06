#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projectionMat * viewMat * modelMat * vec4(aPos, 1.0);
}