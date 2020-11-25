#version 330 core

out vec4 color;

in vec2 aTexCoords;
uniform sampler2D ContainerTex;

void main()
{
    color = texture(ContainerTex, aTexCoords);
}