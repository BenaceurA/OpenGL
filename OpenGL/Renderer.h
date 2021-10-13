#pragma once
#include "Scene.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	//STARTS RECURSIVE RENDERING
	void Render(Scene* scene);
	void Render(SceneNode* node);
private:
	Shader shader;
	Shader depthShader;
	Shader oneColorShader;
};
