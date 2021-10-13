#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <assimp/scene.h>
#include "Shader.h"
#include <vector>
#include <string>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

struct Texture {
	unsigned int id;
	aiTextureType type;
	std::string path;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex>&& vertices,std::vector<unsigned int>&& indices,std::vector<Texture>&& textures) noexcept;
	void Draw(const Shader& shader);
private:
	void drawSetup();

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned VAO, VBO, EBO;
};

