#pragma once
#include "ILight.h"

class SpotLight : ILight
{
public:
	SpotLight(int index, glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,float CutOff,float OuterCutOff);
	~SpotLight() = default;
	void Draw() override;
	void SetShaderUniforms(const Shader& shader) override;
	glm::mat4 GetViewMatrix() override;
	void SetPosition(const Shader& shader,const glm::vec3& position);
	void SetDirection(const Shader& shader,const glm::vec3& direction);
	void SetAmbient(const Shader& shader, glm::vec3& Ambient);
	void SetDiffuse(const Shader& shader, glm::vec3& Diffuse);
	void SetSpecular(const Shader& shader, glm::vec3& Specular);
	void SetCutOff(const Shader& shader, float CutOff);
	void SetOuterCutOff(const Shader& shader, float OuterCutOff);
	glm::vec3 getPosition();
	glm::vec3 getDirection();
private:
	std::string index;
	float CutOff;
	float OuterCutOff;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	const std::string uniformName{ "spotlight" };
};
