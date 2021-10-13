#pragma once
#include <string>
#include <vector>
#include <assimp/scene.h>
#include "Shader.h"
#include "Mesh.h"
#include "SceneNode.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

//the model object creates and deletes it's own meshes
class Model
{
public:
	Model(const std::string& path);
	~Model();
	void SetModelMatrix(glm::mat4 model);
	glm::mat4 GetModelMatrix();
	SceneNode* getModelRootNode();
private:	
	void loadModel(const std::string& path);
	unsigned int loadTexture(const char* path, const char* directory);
	unsigned int loadEmbeddedTexture(const char* textureIndex);
	void loadTextureType(std::vector<Texture>& textures, const aiMaterial* material, const aiTextureType& type);
	SceneNode* processNode(aiNode* node, SceneNode* parent);
	inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4* from);
private:
	static Assimp::Importer import;
	const aiScene* scene; // scene will be delete along with Importer
	SceneNode* rootNode;
	std::vector<Mesh*> meshes;
	std::vector<Texture> loaded_Textures;
	std::string directory;
	std::string name;
	glm::mat4 modelMatrix;
};

