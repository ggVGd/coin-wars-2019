#include "VertexBuffer.h"

namespace Cinnabar
{
	void VertexBuffer::pushAttribute(GLenum type, uint8_t size)
	{
		Attribute attrib;
		attrib.type = type;
		attrib.size = size;
		_attribs.push_back(attrib);
	}
	void VertexBuffer::bind() const
	{
		Buffer::bind(GL_ARRAY_BUFFER);
	}
	void VertexBuffer::setupVAO(std::size_t startIndex) const
	{
		bind();

		const uint32_t stride = _stride();
		char* offset = 0;
		for(std::size_t i = 0; i < _attribs.size(); i++)
		{
			const auto& attrib = _attribs[i];
			glEnableVertexAttribArray(startIndex + i);
			glVertexAttribPointer(
				startIndex + i,
				attrib.size,
				attrib.type,
				GL_FALSE,
				stride,
				offset
			);
			offset += _attributeSize(attrib);
		}
	}
	void VertexBuffer::update(std::size_t size, void* data, GLenum usage)
	{
		bind();
		glBufferData(
			GL_ARRAY_BUFFER,
			size,
			data,
			usage
		);
	}
	uint32_t VertexBuffer::_stride() const
	{
		uint32_t stride = 0;
		for(const auto& attrib : _attribs)
		{
			stride += _attributeSize(attrib);
		}
		return stride;
	}
	uint32_t VertexBuffer::_attributeSize(const Attribute& attrib)
	{
		switch(attrib.type)
		{
			case GL_FLOAT:
				return attrib.size * sizeof(GLfloat);
		}
		return 0;
	}
}
