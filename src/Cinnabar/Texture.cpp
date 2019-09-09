#include "Texture.h"
#include "Logging.h"
#include "ThirdParty/stb_image.h"
#include <cstring>

namespace Cinnabar
{
	Texture::Texture()
	{
		glGenTextures(1, &_handle);

		_texParamI[GL_TEXTURE_WRAP_S]     = GL_REPEAT;
		_texParamI[GL_TEXTURE_WRAP_T]     = GL_REPEAT;
		_texParamI[GL_TEXTURE_MIN_FILTER] = GL_LINEAR;
		_texParamI[GL_TEXTURE_MAG_FILTER] = GL_LINEAR;
	}
	Texture::~Texture()
	{

	}
	const GLuint& Texture::handle() const
	{
		return _handle;
	}
	Texture::operator GLuint() const
	{
		return _handle;
	}
	void Texture::load(const std::string& filename)
	{
		stbi_set_flip_vertically_on_load(true);

		int channels;
		unsigned char* data = stbi_load(filename.c_str(), &_width, &_height, &channels, 4);

		if(!data)
		{
			LOGS(ERROR) << "Failed to load '" << filename << "'";
			return;
		}

		bind(GL_TEXTURE_2D);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	void Texture::bind(const GLenum& target)
	{
		glBindTexture(target, _handle);
		_applyTexParams(target);
	}
	void Texture::setParamF(GLenum param, GLfloat value)
	{
		_texParamF[param] = value;
	}
	void Texture::setParamI(GLenum param, GLint value)
	{
		_texParamI[param] = value;
	}
	const int& Texture::width() const
	{
		return _width;
	}
	const int& Texture::height() const
	{
		return _height;
	}
	void Texture::_applyTexParams(GLenum target)
	{
		for(auto pair : _texParamF)
		{
			glTexParameterf(target, pair.first, pair.second);
		}
		for(auto pair : _texParamI)
		{
			glTexParameteri(target, pair.first, pair.second);
		}
	}
}
