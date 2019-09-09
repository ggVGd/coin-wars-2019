#pragma once
#include "Buffer.h"
#include <vector>

namespace Cinnabar
{
	class VertexBuffer : public Buffer
	{
	public:
		void pushAttribute(GLenum, uint8_t);

		void bind() const;
		void setupVAO(std::size_t = 0) const;

		void update(std::size_t, void*, GLenum = GL_STATIC_DRAW);

		template<typename T>
		void update(std::vector<T> data, GLenum usage = GL_STATIC_DRAW)
		{
			update(data.size() * sizeof(T), data.data(), usage);
		}

	private:
		struct Attribute
		{
			GLenum type;
			uint8_t size;
		};
		std::vector<Attribute> _attribs;

		uint32_t _stride() const;
		static uint32_t _attributeSize(const Attribute&);
	};
}
