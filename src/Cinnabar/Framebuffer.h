#pragma once
#include <GL/glew.h>

namespace Cinnabar
{
	class Framebuffer
	{
	public:
		Framebuffer();
		Framebuffer(const Framebuffer&);
		Framebuffer& operator=(const Framebuffer&);
		~Framebuffer();
		const GLuint& handle() const;
		operator GLuint() const;

		void bind(GLenum = GL_FRAMEBUFFER);
	private:
		unsigned int* _refs;
		GLuint _handle;
	};
}
