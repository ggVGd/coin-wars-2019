#pragma once
#include <GL/glew.h>

namespace Cinnabar
{
	class VertexArray
	{
	public:
		VertexArray();
		VertexArray(const VertexArray&);
		VertexArray& operator=(const VertexArray&);
		~VertexArray();
		const GLuint& handle() const;
		operator GLuint() const;

		void bind() const;
		static void unbind();

	private:
		unsigned int* _refs;
		GLuint _handle;
	};
}
