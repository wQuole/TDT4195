#version 450 core

in vec3 position;
in layout(location=1) vec4 color;
in layout(location=2) vec4 normal;

out layout(location=1) vec4 outColor;
out layout(location=2) vec4 outNormal;

uniform layout(location=3) mat4 M;
//uniform layout(location=4) float mod;

void main()
{
//    mat4x4 identityMatrix;
//    identityMatrix[0] = vec4(1,0,0,0);
//    identityMatrix[1] = vec4(0,1,0,0);
//    identityMatrix[2] = vec4(0,0,1,0);
//    identityMatrix[3] = vec4(0,0,0,1);

    outColor = color;
    outNormal = normal;

    gl_Position = M * vec4(position, 1.0f);
}
