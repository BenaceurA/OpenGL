#version 330 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTex;

uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJ;
uniform mat4 LightVIEW;

out vec3 fragPos;
out vec3 normal;
out vec2 Tex;
out vec4 fragPosLightSpace;

void main()
{
   gl_Position = PROJ*VIEW*MODEL*vec4(vertexPos, 1.0);
   fragPos = vec3(MODEL * vec4(vertexPos, 1.0));
   fragPosLightSpace = PROJ * LightVIEW * vec4(fragPos,1.0);
   normal = normalize(mat3(transpose(inverse(MODEL))) * vertexNormal); //multiply by normal matrix
   Tex = vertexTex;
};