#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 VIEW;
uniform mat4 PROJ;

out vec3 TexCoord;

void main()
{
	TexCoord = aPos;
	vec4 pos = PROJ * VIEW * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
};