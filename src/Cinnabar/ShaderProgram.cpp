#include "ShaderProgram.h"
#include "Logging.h"
#include <cstdlib>

namespace Cinnabar
{
	ShaderProgram::ShaderProgram()
	{
		_handle = glCreateProgram();
	}
	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(_handle);
	}
	ShaderProgram::operator GLuint() const
	{
		return _handle;
	}
	const GLuint& ShaderProgram::handle() const
	{
		return _handle;
	}
	void ShaderProgram::attach(const GLuint& shader)
	{
		glAttachShader(_handle, shader);
	}
	void ShaderProgram::attachSource(const GLenum& type, const char* source)
	{
		Shader shader(type);
		shader.loadSource(source);
		attach(shader);
	}
	void ShaderProgram::link()
	{
		glLinkProgram(_handle);

		int success;
		char info[512];
		glGetProgramiv(_handle, GL_LINK_STATUS, &success);
		if(success == GL_FALSE)
		{
			glGetProgramInfoLog(_handle, 512, NULL, info);
			LOGS(ERROR) << "Shader linking error: " << info;
			exit(-1);
		}
	}
	void ShaderProgram::use()
	{
		glUseProgram(_handle);
	}
	GLint ShaderProgram::uniformLocation(const char* name)
	{
		return glGetUniformLocation(_handle, name);
	}
	void ShaderProgram::setUniform(const char* name, const GLint& value)
	{
		GLint location = uniformLocation(name);
		glUniform1i(location, value);
	}
	void ShaderProgram::setUniform(const char* name, const GLuint& value)
	{
		GLint location = uniformLocation(name);
		glUniform1ui(location, value);
	}
	void ShaderProgram::setUniform(const char* name, const float& value)
	{
		GLint location = uniformLocation(name);
		glUniform1f(location, value);
	}
	void ShaderProgram::setUniform(const char* name, const glm::vec2& value)
	{
		GLint location = uniformLocation(name);
		glUniform2fv(location, 1, glm::value_ptr(value));
	}
	void ShaderProgram::setUniform(const char* name, const glm::vec3& value)
	{
		GLint location = uniformLocation(name);
		glUniform3fv(location, 1, glm::value_ptr(value));
	}
	void ShaderProgram::setUniform(const char* name, const glm::vec4& value)
	{
		GLint location = uniformLocation(name);
		glUniform4fv(location, 1, glm::value_ptr(value));
	}
	void ShaderProgram::setUniform(const char* name, const glm::mat4& value)
	{
		GLint location = uniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
	void ShaderProgram::setUniform(const char* name, const Vector2& value)
	{
		GLint location = uniformLocation(name);
		glUniform2fv(location, 1, &value.x);
	}
	void ShaderProgram::setUniform(const char* name, const Vector3& value)
	{
		GLint location = uniformLocation(name);
		glUniform3fv(location, 1, &value.x);
	}
	void ShaderProgram::setUniform(const char* name, const Vector4& value)
	{
		GLint location = uniformLocation(name);
		glUniform4fv(location, 1, &value.x);
	}
	void ShaderProgram::setUniform(const char* name, const Matrix4& value)
	{
		GLint location = uniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, value.data);
	}
	GLint ShaderProgram::attribLocation(const char* name)
	{
		return glGetAttribLocation(_handle, name);
	}
}
