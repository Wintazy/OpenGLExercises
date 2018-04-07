#version 330

in vec2 TexCoords;

out vec4 color;

uniform sampler2D glyphTex;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(glyphTex, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
} 