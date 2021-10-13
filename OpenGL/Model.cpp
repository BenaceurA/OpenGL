#include "Model.h"

#include <glad/glad.h>
#include "stb_image.h"
#include <iostream>
#include "Instrumentation.h"

Assimp::Importer Model::import;

Model::Model(const std::string& name)
	:
	name(name),
	directory("assets/" + name + "/" + name + ".fbx")
{
	rootNode = new SceneNode(nullptr);
	try
	{
		loadModel(directory);
	}
	catch (const std::exception& e)
	{
		std::cout<< e.what() << " | " << __FILE__ << " | " << __LINE__ << std::endl;
	}
	
}

Model::~Model()
{
	std::cout << "-------model destructor called-------" << std::endl;
	for (Mesh* m : meshes)
	{
		delete m;
	}
	if (rootNode != nullptr)
	{
		delete rootNode;
		rootNode = nullptr;
	}
}

void Model::SetModelMatrix(glm::mat4 model)
{
	this->modelMatrix = model;
}

glm::mat4 Model::GetModelMatrix()
{
	return this->modelMatrix;
}

SceneNode* Model::getModelRootNode()
{
	return rootNode;
}

void Model::loadModel(const std::string& path)
{
	FUNCTION_PROFILE();
	scene = Model::import.ReadFile(path, aiProcess_Triangulate);
	if (scene != nullptr)
	{
		rootNode->AddChild(processNode(scene->mRootNode, rootNode)); //start loading from asssimp scene structure to model rootNode
	}
	else
	{
		throw std::runtime_error("scene was nullptr");
	}
}

void Model::loadTextureType(std::vector<Texture>& textures,const aiMaterial* material, const aiTextureType& type)
{
	FUNCTION_PROFILE();
	unsigned int textureCount = material->GetTextureCount(type);
	for (size_t i = 0; i < textureCount; i++)
	{
		aiString texture_location;

		material->GetTexture(type, i, &texture_location);

		bool skip = false;
		for (size_t j = 0; j < loaded_Textures.size(); j++)
		{
			if (strcmp(texture_location.C_Str(), loaded_Textures[j].path.data()) == 0)
			{
				textures.push_back(loaded_Textures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			if (texture_location.data[0] != '*') //not embeded texture
			{
				texture.id = loadTexture(texture_location.C_Str(), ("assets/" + name + "/").c_str());
			}
			else
			{
				texture.id = loadEmbeddedTexture(&texture_location.data[1]);
			}
			texture.type = type;
			texture.path = texture_location.C_Str();
			textures.push_back(texture);
			std::cout << texture.path << std::endl; //texture was loaded
			loaded_Textures.push_back(std::move(texture));
		}
	}
}

unsigned int Model::loadTexture(const char* path,const char* directory)
{
	FUNCTION_PROFILE();
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	std::string p{ path };
	std::string d{ directory };
	std::string newPath = d + p;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(newPath.c_str(), &width, &height, &nrComponents, 0);
	
	if (data)
	{
		GLenum format = GL_RED;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		

		glBindTexture(GL_TEXTURE_2D, textureID);
		
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

unsigned int Model::loadEmbeddedTexture(const char* textureIndex)
{
	FUNCTION_PROFILE();
	unsigned int textureID;
	glGenTextures(1, &textureID);

	aiTexture* texture = scene->mTextures[atoi(textureIndex)];

	if (texture)
	{
		if (!(texture->CheckFormat("jpg")||texture->CheckFormat("png")))
		{
			throw std::exception("embedded texture was not compressed");
		}
		glBindTexture(GL_TEXTURE_2D, textureID);
		int width, height, nrComponents;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load_from_memory(reinterpret_cast<stbi_uc*>(texture->pcData), texture->mWidth, &width, &height, &nrComponents, 0);

		GLenum format = GL_RED;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	return textureID;
}

SceneNode* Model::processNode(aiNode* node, SceneNode* parent)
{	
	FUNCTION_PROFILE();

	//	this function is recursive
	//	we start from the rootNode in aiScene we create a sceneNode for it, if it has any aiMesh we load them into Mesh 
	//	and store a pointer to it in this Model object and it's sceneNode Hierarchy then recursively call processNode for all aiNode's children if it has any

	SceneNode* sceneNode = new SceneNode(parent);
	sceneNode->SetTransformation(aiMatrix4x4ToGlm(&node->mTransformation));

	for (unsigned int k = 0; k < node->mNumMeshes; k++)
	{
		int meshIndex = node->mMeshes[k];
		aiMesh* mesh = scene->mMeshes[meshIndex];		
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			vertex.position.r = mesh->mVertices[i].x;
			vertex.position.g = mesh->mVertices[i].y;
			vertex.position.b = mesh->mVertices[i].z;
			if (mesh->HasNormals())
			{
				vertex.normal.r = mesh->mNormals[i].x;
				vertex.normal.g = mesh->mNormals[i].y;
				vertex.normal.b = mesh->mNormals[i].z;
			}

			if (mesh->HasTextureCoords(0))
			{
				vertex.texCoord.r = mesh->mTextureCoords[0][i].x;
				vertex.texCoord.g = mesh->mTextureCoords[0][i].y;
			}
			vertices.push_back(vertex);
		}

		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (size_t j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			loadTextureType(textures, material, aiTextureType_DIFFUSE);
			loadTextureType(textures, material, aiTextureType_SPECULAR);
		}

		//mesh gets created here
		Mesh* m = new Mesh{ std::move(vertices),std::move(indices),std::move(textures) }; 

		//a pointer to it get pushed into a vector
		meshes.push_back(m); 

		// we add the mesh pointer into the scene sceneNode
		sceneNode->AddMesh(m); 

	} 
	
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		sceneNode->AddChild(processNode(node->mChildren[i],sceneNode));		
	}

	return sceneNode;
}

inline glm::mat4 Model::aiMatrix4x4ToGlm(const aiMatrix4x4* from)
{
	glm::mat4 to;

	to[0][0] = (GLfloat)from->a1; to[0][1] = (GLfloat)from->b1;  to[0][2] = (GLfloat)from->c1; to[0][3] = (GLfloat)from->d1;
	to[1][0] = (GLfloat)from->a2; to[1][1] = (GLfloat)from->b2;  to[1][2] = (GLfloat)from->c2; to[1][3] = (GLfloat)from->d2;
	to[2][0] = (GLfloat)from->a3; to[2][1] = (GLfloat)from->b3;  to[2][2] = (GLfloat)from->c3; to[2][3] = (GLfloat)from->d3;
	to[3][0] = (GLfloat)from->a4; to[3][1] = (GLfloat)from->b4;  to[3][2] = (GLfloat)from->c4; to[3][3] = (GLfloat)from->d4;

	return to;
}
