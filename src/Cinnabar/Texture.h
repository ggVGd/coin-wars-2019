#pragma once
#include <GL/glew.h>
#include <memory>
#include <unordered_map>

namespace Cinnabar
{
	class Texture
	{
	public:
		Texture();
		~Texture();
		const GLuint& handle() const;
		operator GLuint() const;

		void load(const std::string&);

		void bind(const GLenum& = GL_TEXTURE_2D);

		void setParamF(GLenum, GLfloat);
		void setParamI(GLenum, GLint);
		
		const int& height() const;
		const int& width() const;

	private:
		GLuint _handle;

		std::unordered_map<GLenum, GLfloat> _texParamF;
		std::unordered_map<GLenum, GLint>   _texParamI;
		void _applyTexParams(GLenum);

		int _width;
		int _height;

	};
}
