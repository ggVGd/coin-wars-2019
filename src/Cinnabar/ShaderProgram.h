#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Shader.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4.h"

namespace Cinnabar
{
	class Shader;

	class ShaderProgram
	{
	public:
		ShaderProgram();
		~ShaderProgram();
		const GLuint& handle() const;
		operator GLuint() const;

		void attach(const GLuint&);
		void attachSource(const GLenum&, const char*);
		void link();

		void use();
		
		GLint uniformLocation(const char*);
		void setUniform(const char*, const GLint&);
		void setUniform(const char*, const GLuint&);
		void setUniform(const char*, const float&);
		void setUniform(const char*, const glm::vec2&);
		void setUniform(const char*, const glm::vec3&);
		void setUniform(const char*, const glm::vec4&);
		void setUniform(const char*, const glm::mat4&);
		void setUniform(const char*, const Vector2&);
		void setUniform(const char*, const Vector3&);
		void setUniform(const char*, const Vector4&);
		void setUniform(const char*, const Matrix4&);

		GLint attribLocation(const char*);

	private:
		GLuint _handle;
	};
}
