#version 410 core

in vec3 position;
in vec4 inColor;
out vec4 position;

void main()
{
    gl_Position = vec4(position, 1.0f);
}
