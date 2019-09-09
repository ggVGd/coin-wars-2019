#include "VertexArray.h"
#include <GL/glew.h>

namespace Cinnabar
{
	VertexArray::VertexArray()
	{
		_refs = new unsigned int;
		*_refs = 1;

		glGenVertexArrays(1, &_handle);
	}
	VertexArray::VertexArray(const VertexArray& other)
	{
		_handle = other._handle;
		_refs = other._refs;
		(*_refs)++;
	}
	VertexArray& VertexArray::operator=(const VertexArray& other)
	{
		_handle = other._handle;
		_refs = other._refs;
		(*_refs)++;
		return *this;
	}
	VertexArray::~VertexArray()
	{
		(*_refs)--;
		if(*_refs == 0)
		{
			glDeleteVertexArrays(1, &_handle);
		}
	}
	const GLuint& VertexArray::handle() const
	{
		return _handle;
	}
	VertexArray::operator GLuint() const
	{
		return _handle;
	}
	void VertexArray::bind() const
	{
		glBindVertexArray(_handle);
	}
	void VertexArray::unbind()
	{
		glBindVertexArray(0);
	}
}
