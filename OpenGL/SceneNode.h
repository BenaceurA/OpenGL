#pragma once
#include <vector>
#include "Mesh.h"
#include <memory>

class SceneNode
{
public:
	SceneNode(SceneNode* parent,bool deleteChildren = true);
	~SceneNode();
	void AddChild(SceneNode* Node);
	void AddMesh(Mesh* mesh);
	void SetWorldTransformation(glm::mat4 tranformation);
	void SetTransformation(glm::mat4 transformation);
	void Draw(const Shader& shader);
	std::vector<SceneNode*> GetChildren();
	std::vector<Mesh*> GetMeshes();
	int GetNumMeshes();
	int GetNumChildren();
private:
	SceneNode* parent;
	std::vector<SceneNode*> children;
	std::vector<Mesh*> meshes;

	//RELATIVE TO THE PARENT
	glm::mat4 transformation;
	glm::mat4 worldTransformation;
	
	//FOR THE SCENE ROOTNODE
	bool deleteChildren;
};
