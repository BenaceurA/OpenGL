#version 330 core

out vec4 FragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 Tex;


void main()
{
    FragColor = vec4(0.0,1.0,1.0,1.0);
};