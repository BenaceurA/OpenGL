#include "SpotLight.h"

SpotLight::SpotLight(int index,glm::vec3 position,glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float CutOff, float OuterCutOff)
	:
	index(std::to_string(index)),
	position(position),
	direction(glm::normalize(direction)),
	ambient(ambient),
	diffuse(diffuse),
	specular(specular),
	CutOff(CutOff),
	OuterCutOff(OuterCutOff)
{
}

void SpotLight::Draw()
{
}

void SpotLight::SetShaderUniforms(const Shader& shader)
{
	shader.setUniformFloat(uniformName + "[" + index + "].cutOff", glm::cos(glm::radians(CutOff)));
	shader.setUniformFloat(uniformName + "[" + index + "].outerCutOff", glm::cos(glm::radians(OuterCutOff)));
	shader.setVec3(uniformName + "[" + index + "].ambient", ambient);
	shader.setVec3(uniformName + "[" + index + "].diffuse", diffuse);
	shader.setVec3(uniformName + "[" + index + "].specular", specular);
}

glm::mat4 SpotLight::GetViewMatrix()
{
	glm::vec3 Right = glm::normalize(glm::cross(direction, { 0.0,1.0,0.0 }));
	glm::vec3 Up = glm::normalize(glm::cross(Right, direction));	
	return glm::lookAt(position, position + direction, Up);
}

void SpotLight::SetPosition(const Shader& shader,const glm::vec3& position)
{
	this->position = position;
	shader.setVec4(uniformName + "[" + index + "].position", { position,1.0 });
}

void SpotLight::SetDirection(const Shader& shader,const glm::vec3& direction)
{
	this->direction = direction;
	shader.setVec3(uniformName + "[" + index + "].direction", direction);
}

glm::vec3 SpotLight::getPosition()
{
	return position;
}

glm::vec3 SpotLight::getDirection()
{
	return direction;
}
