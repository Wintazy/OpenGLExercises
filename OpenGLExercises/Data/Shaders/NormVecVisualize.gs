#version 330
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.4;

void main()
{
	vec4 centerPosition = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3;
	vec3 averageNormVec = (gs_in[0].normal + gs_in[1].normal + gs_in[2].normal) / 3;
    gl_Position = centerPosition;
	EmitVertex();
    gl_Position = centerPosition + vec4(averageNormVec, 0.0) * MAGNITUDE;
    EmitVertex();
    EndPrimitive();
}