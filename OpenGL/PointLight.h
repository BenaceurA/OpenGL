#pragma once
#include "ILight.h"

class PointLight : ILight
{
public:
	PointLight(int index, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~PointLight() = default;
	void Draw() override;
	void SetShaderUniforms(const Shader& shader) override;
	glm::mat4 GetViewMatrix() override;
private:
	std::string index;
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	const std::string uniformName{ "pointlight" };
};
