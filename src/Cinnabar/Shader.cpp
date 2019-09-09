#include "Shader.h"
#include "Logging.h"
#include <cstdlib>

namespace Cinnabar
{
	Shader::Shader(const GLenum& type) : _type(type)
	{
		_handle = glCreateShader(type);
	}
	Shader::~Shader()
	{
		glDeleteShader(_handle);
	}
	const GLenum& Shader::type() const
	{
		return _type;
	}
	const GLuint& Shader::handle() const
	{
		return _handle;
	}
	Shader::operator GLuint() const
	{
		return _handle;
	}
	void Shader::loadSource(const char* source)
	{
		_compile(source);
	}
	void Shader::_compile(const char* source)
	{
		glShaderSource(_handle, 1, &source, nullptr);
		glCompileShader(_handle);

		int success;
		glGetShaderiv(_handle, GL_COMPILE_STATUS, &success);
		if(success == GL_FALSE)
		{
			char info[512];
			glGetShaderInfoLog(_handle, 512, nullptr, info);
			LOGS(ERROR) << "Shader compilation error: " << info;
		}
	}
}
