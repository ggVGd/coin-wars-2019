#include "SceneRenderer.h"
#include "Scene.h"
#include <algorithm>

SceneRenderer::SceneRenderer()
{
	_shader.attachSource(GL_VERTEX_SHADER, R"(
		#version 120
		attribute vec2 position;
		attribute vec2 texcoord;
		attribute vec3 color;
		varying vec2 frag_texcoord;
		varying vec3 frag_color;
		uniform mat4 MVP;
		void main()
		{
			frag_texcoord = texcoord;
			frag_color = color;
			gl_Position = MVP * vec4(position, 0, 1);
		})");

	_shader.attachSource(GL_FRAGMENT_SHADER, R"(
		#version 120
		varying vec2 frag_texcoord;
		varying vec3 frag_color;
		uniform sampler2D texture0;
		void main()
		{
			gl_FragColor = texture2D(texture0, frag_texcoord) * vec4(frag_color, 1);
		})");
		
	_shader.link();
}
void SceneRenderer::setScene(Scene* scene)
{
	_scene = scene;
}
void SceneRenderer::render(const glm::mat4& MVP)
{
	_shader.use();
	_shader.setUniform("MVP", MVP);

	std::sort(
		_scene->sprites.begin(),
		_scene->sprites.end(),
		[](auto a, auto b) -> bool
		{
			return a->zIndex() < b->zIndex();
		}
	);

	for(auto& sprite : _scene->sprites)
	{
		sprite->texture()->bind();
		sprite->render();
	}
}
