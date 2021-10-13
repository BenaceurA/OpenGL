#include "Renderer.h"

Renderer::Renderer()
	:
	shader("vertex.glsl", "fragment.glsl"),
	depthShader("depthVertex.glsl", "depthFragment.glsl"),
	oneColorShader("vertex.glsl", "lightFragment.glsl")
{
}

void Renderer::Render(Scene* scene)
{
	//rootnode of a scene never contains meshes
	Render(scene->GetRootNode());
}

void Renderer::Render(SceneNode* node)
{
	for (Mesh* mesh : node->GetMeshes())
	{
		mesh->Draw(shader); // this not how it should be .... the mesh shouldn't have a draw method
	}
	for (SceneNode* node: node->GetChildren())
	{
		Render(node);
	}
}
