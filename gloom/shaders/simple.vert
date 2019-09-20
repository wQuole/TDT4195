#version 450 core

in vec3 position;
layout(location=1) in vec4 vertexColor;
uniform layout(location=3) mat4 m;
uniform layout(location=2) float mod;

layout(location=1) out vec4 outVertexColor;

void main()
{
    mat4x4 identityMatrix;
    identityMatrix[0] = vec4(1,0,0,0);
    identityMatrix[1] = vec4(0,1,0,0);
    identityMatrix[2] = vec4(0,0,1,0);
    identityMatrix[3] = vec4(0,0,0,1);
    outVertexColor = vertexColor;
    gl_Position = vec4(position, 1.0f) * m;
}
