#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(int index, glm::vec3 direction, glm::vec3 ambient = {0.3,0.3,0.3}, glm::vec3 diffuse = { 1.0,1.0,1.0 }, glm::vec3 specular = { 0.8,0.8,0.8 })
	:
	index(std::to_string(index)),
	direction(direction),
	ambient(ambient),
	diffuse(diffuse),
	specular(specular)
{
}

void DirectionalLight::Draw() {

}

void DirectionalLight::SetShaderUniforms(const Shader& shader)
{
	shader.setVec3(uniformName + "[" + index + "].ambient", ambient);
	shader.setVec3(uniformName + "[" + index + "].diffuse", diffuse);
	shader.setVec3(uniformName + "[" + index + "].specular", specular);
}

glm::mat4 DirectionalLight::GetViewMatrix()
{
	return glm::mat4();
}
