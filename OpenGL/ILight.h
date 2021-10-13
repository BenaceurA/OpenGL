#pragma once
#include "Shader.h"

class ILight abstract {
public:
	ILight() = default;
	virtual ~ILight() = default;
	virtual void Draw() = 0;
	virtual void SetShaderUniforms(const Shader& shader) = 0;
	virtual glm::mat4 GetViewMatrix() = 0;
protected:
	glm::vec3 Position;
};