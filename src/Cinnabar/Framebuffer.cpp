#include "Framebuffer.h"
#include <GL/glew.h>

namespace Cinnabar
{
	Framebuffer::Framebuffer()
	{
		_refs = new unsigned int;
		*_refs = 1;

		glGenFramebuffers(1, &_handle);
	}
	Framebuffer::Framebuffer(const Framebuffer& other)
	{
		_handle = other._handle;
		_refs = other._refs;
		(*_refs)++;
	}
	Framebuffer& Framebuffer::operator=(const Framebuffer& other)
	{
		_handle = other._handle;
		_refs = other._refs;
		(*_refs)++;
		return *this;
	}
	Framebuffer::~Framebuffer()
	{
		(*_refs)--;
		if(*_refs == 0)
		{
			glDeleteFramebuffers(1, &_handle);
		}
	}
	const GLuint& Framebuffer::handle() const
	{
		return _handle;
	}
	Framebuffer::operator GLuint() const
	{
		return _handle;
	}
	void Framebuffer::bind(GLenum target)
	{
		glBindFramebuffer(target, _handle);
	}
}
