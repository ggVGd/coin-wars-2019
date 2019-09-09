#include "Buffer.h"
#include <GL/glew.h>

namespace Cinnabar
{
	Buffer::Buffer()
	{
		_refs = new unsigned int;
		*_refs = 1;

		glGenBuffers(1, &_handle);
	}
	Buffer::Buffer(const Buffer& other)
	{
		_handle = other._handle;
		_refs = other._refs;
		(*_refs)++;
	}
	Buffer& Buffer::operator=(const Buffer& other)
	{
		_handle = other._handle;
		_refs = other._refs;
		(*_refs)++;
		return *this;
	}
	Buffer::~Buffer()
	{
		(*_refs)--;
		if(*_refs == 0)
		{
			glDeleteBuffers(1, &_handle);
		}
	}
	const GLuint& Buffer::handle() const
	{
		return _handle;
	}
	Buffer::operator GLuint() const
	{
		return _handle;
	}
	void Buffer::bind(GLenum target) const
	{
		glBindBuffer(target, _handle);
	}
}
