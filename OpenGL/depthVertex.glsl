#version 330 core

layout (location = 0) in vec3 vertexPos;


uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJ;

void main()
{
   gl_Position = PROJ*VIEW*MODEL*vec4(vertexPos, 1.0);
};