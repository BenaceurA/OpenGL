#include "PointLight.h"

PointLight::PointLight(int index, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	:
	index(std::to_string(index)),
	position(position),
	ambient(ambient),
	diffuse(diffuse),
	specular(specular)
{
}

void PointLight::Draw()
{
}

void PointLight::SetShaderUniforms(const Shader& shader)
{
	shader.setVec4(uniformName + "[" + index + "].position", { position,1.0 });
	shader.setVec3(uniformName + "[" + index + "].ambient", ambient);
	shader.setVec3(uniformName + "[" + index + "].diffuse", diffuse);
	shader.setVec3(uniformName + "[" + index + "].specular", specular);
}

glm::mat4 PointLight::GetViewMatrix()
{
	return glm::mat4();
}
