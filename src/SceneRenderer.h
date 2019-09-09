#pragma once
#include "Cinnabar/ShaderProgram.h"

class Scene;

class SceneRenderer
{
public:
	SceneRenderer();
	void setScene(Scene* scene);

	void render(const glm::mat4& MVP);

private:
	Cinnabar::ShaderProgram _shader;
	Scene* _scene;
};
