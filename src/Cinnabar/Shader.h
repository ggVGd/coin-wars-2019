#pragma once
#include <GL/glew.h>

namespace Cinnabar
{
	class Shader
	{
	public:
		Shader(const GLenum&);
		~Shader();
		const GLenum& type() const;
		const GLuint& handle() const;
		operator GLuint() const;

		void loadSource(const char*);

	private:
		GLenum _type;
		GLuint _handle;

		void _compile(const char*);
	};
}
