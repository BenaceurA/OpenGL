#pragma once
#include "ILight.h"

class DirectionalLight : ILight {
public:
	DirectionalLight(int index, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~DirectionalLight() = default;
	void Draw() override;
	void SetShaderUniforms(const Shader& shader) override;
	glm::mat4 GetViewMatrix() override;
private:
	std::string index;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	const std::string uniformName{ "dirlight" };
};