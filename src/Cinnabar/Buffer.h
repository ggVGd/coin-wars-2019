#pragma once
#include <GL/glew.h>

namespace Cinnabar
{
	class Buffer
	{
	public:
		Buffer();
		Buffer(const Buffer&);
		Buffer& operator=(const Buffer&);
		~Buffer();
		const GLuint& handle() const;
		operator GLuint() const;

		void bind(GLenum) const;
	private:
		unsigned int* _refs;
		GLuint _handle;
	};
}
