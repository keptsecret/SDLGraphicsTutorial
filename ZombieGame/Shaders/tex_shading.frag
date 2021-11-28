#version 130

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

// uniforms allow global variables across all vertices in a mesh
uniform sampler2D samplerTex;

void main()
{
    vec4 textureColor = texture(samplerTex, fragmentUV);

    //color = textureColor * fragmentColor;
    color = fragmentColor * textureColor;
}