#version 450 core

in layout(location=1) vec4 color;
in layout(location=2) vec4 normal;

out vec4 fragColor;

void main()
{
    fragColor = normal;
}


