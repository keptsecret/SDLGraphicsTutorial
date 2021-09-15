#version 130

in vec2 fragmentPosition;
in vec4 fragmentColor;

out vec4 color;

// uniforms allow golbal variables across all vertices in a mesh
uniform float time;

void main()
{
    color = vec4(fragmentColor.r * (cos(fragmentPosition.x * 4.0 + time) + 1.0) * 0.5,
                fragmentColor.g * (cos(fragmentPosition.y * 8.0 + time) + 1.0) * 0.5,
                fragmentColor.b * (cos(fragmentPosition.x * 2.0 + time) + 1.0) * 0.5,
                fragmentColor.a);
}