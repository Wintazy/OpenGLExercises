#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 projectionMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

void main()
{
    gl_Position = projectionMat * viewMat * modelMat * vec4(aPos, 1.0); 
    mat3 normalMatrix = mat3(transpose(inverse(viewMat * modelMat)));
    vs_out.normal = normalize(vec3(projectionMat * vec4(normalMatrix * aNormal, 0.0)));
}