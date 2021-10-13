#pragma once
#include <string>
#include "Shader.h"
#include <vector>
class Cubemap
{
public:
	Cubemap(const std::vector<std::string>& faces, const std::string directory);
	void Draw();
private:
	void Setup();
	std::vector<std::string> faces;
	std::string directory;
	unsigned int textureID;
	unsigned int VBO, VAO;
};

