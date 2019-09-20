#version 450 core

in layout(location=1) vec4 vertexColor;

out vec4 fragColor;

void main()
{
    fragColor = vertexColor;
}
