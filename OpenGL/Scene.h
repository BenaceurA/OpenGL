#pragma once
#include <vector>
#include "Model.h"
#include "Camera.h"
#include "SpotLight.h"
#include "PointLight.h"

class Scene
{
public:
	Scene(Camera& camera);
	~Scene();
	void Draw(unsigned int depthmap,float x, float z);
	void DrawShadowMap();
	SceneNode* GetRootNode();
	void test();
private:
	void setupScene();
	SceneNode* rootNode;
	Shader shader;
	Shader depthShader;
	Shader oneColorShader;
	Camera& camera;
	SpotLight* spotlight =  new SpotLight( 0,camera.Position,camera.Front, { 0.1,0.1,0.1 }, { 1.0,1.0,1.0 }, { 0.8,0.8,0.8 }, 15.0f, 35.0f );
	std::vector<ILight*> lights = { reinterpret_cast<ILight*>(spotlight) };
};

