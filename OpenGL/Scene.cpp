#include "Scene.h"
#include <glad/glad.h>
#include <iostream>


Scene::Scene(Camera& camera)
	:
	shader("vertex.glsl", "fragment.glsl"),
	depthShader("depthVertex.glsl","depthFragment.glsl"),
	oneColorShader("vertex.glsl", "lightFragment.glsl"),
	camera(camera)
{
	rootNode = new SceneNode(nullptr,false);

	depthShader.use();
	depthShader.setMat4("PROJ", glm::perspective(camera.fov, camera.aspect, 0.1f, 200.0f));

	shader.use();
	shader.setMat4("PROJ", glm::perspective(camera.fov, camera.aspect, 0.1f, 200.0f));
	shader.setUniformInt("spotlightCount", 1);
	shader.setUniformInt("dirlightCount", 0);
	shader.setUniformInt("pointlightCount", 0);	
	for (ILight* light : lights)
	{
		light->SetShaderUniforms(shader);
	}
	spotlight->SetDirection(shader, glm::normalize(glm::vec3{ -1.0,-1.0,0.0 }));
}

Scene::~Scene()
{
	std::cout << "-------scene destructor called-------" << std::endl;
	//delete pointlight;
	delete spotlight;

	delete rootNode;	//each model takes care of deleting it's root node and children nodes
						//this wont delete it's children (model nodes)
}

void Scene::Draw(unsigned int depthmap,float x, float z)
{
	shader.use();
	shader.setVec3("viewPos", camera.Position);
	shader.setMat4("VIEW", camera.GetViewMatrix());
	shader.setMat4("LightVIEW", spotlight->GetViewMatrix());
	shader.setUniformInt("depthMap", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthmap);
	spotlight->SetPosition(shader, { 0.0f+x,15.0f,0.0f+z });
	rootNode->Draw(shader);
}

void Scene::DrawShadowMap()
{
	depthShader.use();
	depthShader.setMat4("VIEW", spotlight->GetViewMatrix());

	rootNode->Draw(depthShader);
}

SceneNode* Scene::GetRootNode()
{
	return rootNode;
}


