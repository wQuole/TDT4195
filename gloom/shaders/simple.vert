#version 450 core

in vec3 position;
layout(location=1) in vec4 vertexColor;

// uniform layout(location=4) mat4 m;
layout(location=1) out vec4 outVertexColor;

void main()
{
    outVertexColor = vertexColor;

    //mat4x4 identityMatrix = mat4(1);
    gl_Position = vec4(position, 1.0f) ;
}
