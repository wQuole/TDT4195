#version 450 core

in layout(location=1) vec4 color;
in layout(location=2) vec3 normal;

out vec4 fragColor;

vec3 lightDirection = normalize(vec3(0.8, -0.5, 0.6));

void main()
{
    fragColor = color * max(0.0f, dot(normal, -lightDirection));
}