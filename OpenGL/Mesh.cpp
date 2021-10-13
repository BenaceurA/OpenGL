#include "Mesh.h"
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices,std::vector<Texture>&& textures) noexcept
	:
	vertices(std::move(vertices)),
	indices(std::move(indices)),
	textures(std::move(textures))
{
	drawSetup();
}

void Mesh::Draw(const Shader& shader)
{
	unsigned int diffuseCounter = 0;
	unsigned int specularCounter = 0;
	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		std::string number;
		std::string name;
		if (textures[i].type == aiTextureType_DIFFUSE)
		{
			number = std::to_string(diffuseCounter++);
			name = "texture_diffuse";
		}

		else if (textures[i].type == aiTextureType_SPECULAR)
		{
			number = std::to_string(specularCounter++);
			name = "texture_specular";

		}
		shader.setUniformInt("material." + name + number, i+1);
	}
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::drawSetup()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

}