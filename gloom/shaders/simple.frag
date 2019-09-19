#version 410 core

in layout(location=2) vec4 vertexColor;

out vec4 fragColor;

void main()
{
    fragColor = vertexColor;
}
