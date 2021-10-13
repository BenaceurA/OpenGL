#include "SceneNode.h"
#include <iostream>

SceneNode::SceneNode(SceneNode* parent, bool deleteChildren)
	:
	parent(parent),
	deleteChildren(deleteChildren),
	transformation(glm::mat4(1.0f)),
	worldTransformation(glm::mat4(1.0f))
{
}

SceneNode::~SceneNode()
{
	if (deleteChildren)
	{
		for (SceneNode* child : children)
		{
			if (child != nullptr)
			{
				delete child;
				child = nullptr;
			}
		}
	}	
}

void SceneNode::AddChild(SceneNode* Node)
{
	Node->parent = this;
	children.push_back(Node);
}

void SceneNode::AddMesh(Mesh* mesh)
{
	meshes.push_back(mesh);
}

void SceneNode::SetWorldTransformation(glm::mat4 tranformation)
{
	worldTransformation = tranformation;
	for (SceneNode* child : children)
	{
		child->SetWorldTransformation(tranformation);
	}	
}

void SceneNode::SetTransformation(glm::mat4 transformation)
{
	this->transformation = transformation;
}

void SceneNode::Draw(const Shader& shader)
{
	shader.setMat4("MODEL", worldTransformation);
	for (auto mesh : meshes)
	{
		mesh->Draw(shader);
	}
	for (SceneNode* child : children)
	{
		child->Draw(shader);
	}
}

std::vector<SceneNode*> SceneNode::GetChildren()
{
	return children;
}

std::vector<Mesh*> SceneNode::GetMeshes()
{
	return meshes;
}

int SceneNode::GetNumMeshes()
{
	return meshes.size();
}

int SceneNode::GetNumChildren()
{
	return children.size();
}
