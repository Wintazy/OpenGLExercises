#version 330
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 instanceMatrix;

out vec2 TexCoords;

uniform mat4 projectionMat;
uniform mat4 viewMat;

void main()
{
    gl_Position = projectionMat * viewMat * instanceMatrix * vec4(aPos, 1.0); 
    TexCoords = aTexCoords;
}