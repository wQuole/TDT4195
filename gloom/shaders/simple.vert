#version 450 core

in vec3 position;
in layout(location=1) vec4 color;
in layout(location=2) vec3 normal;

out layout(location=1) vec4 outColor;
out layout(location=2) vec3 outNormal;

uniform layout(location=3) mat4 M;
uniform layout(location=4) mat4 modelMatrix;


void main()
{
    outColor = color;
    outNormal = normalize(mat3(modelMatrix) * normal);

    gl_Position = M * vec4(position, 1.0f);
}