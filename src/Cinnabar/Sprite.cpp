#include "Sprite.h"
#include <glm/vec4.hpp>
#include <cmath>

namespace Cinnabar
{
	namespace
	{
		glm::vec2 rotatePoint(glm::vec2 point, glm::vec2 center, float angle)
		{
			const float sinA = sin(angle);
			const float cosA = cos(angle);

			point -= center;

			return center + glm::vec2(
				point.x * cosA - point.y * sinA,
				point.x * sinA + point.y * cosA
			);
		}
	}

	Sprite::Sprite()
	{
		_vao.bind();

		_vbo.bind(GL_ARRAY_BUFFER);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));

		_vao.unbind();

		_dirty = true;
	}
	std::shared_ptr<Texture> Sprite::texture() const
	{
		return _texture;
	}
	void Sprite::setTexture(std::shared_ptr<Texture> texture)
	{
		_texture = texture;
	}
	const glm::vec2& Sprite::position() const
	{
		return _position;
	}
	const bool& Sprite::centeredPosition() const
	{
		return _centeredPosition;
	}
	void Sprite::setCenteredPosition(bool x)
	{
		_centeredPosition = x;
	}
	void Sprite::setPosition(const float& x, const float& y)
	{
		if(_position.x != x || _position.y != y)
		{
			_position.x = x;
			_position.y = y;
			_dirty = true;
		}
	}
	void Sprite::setPosition(const glm::vec2& position)
	{
		setPosition(position.x, position.y);
	}
	const int& Sprite::zIndex() const
	{
		return _zIndex;
	}
	void Sprite::setZIndex(int z)
	{
		_zIndex = z;
	}
	const float& Sprite::rotation() const
	{
		return _rotation;
	}
	void Sprite::setRotation(const float& rotation)
	{
		if(_rotation != rotation)
		{
			_rotation = rotation;
			_dirty = true;
		}
	}
	const glm::vec2& Sprite::size() const
	{
		return _size;
	}
	void Sprite::setSize(const float& x, const float& y)
	{
		if(_size.x != x || _size.y != y)
		{
			_size.x = x;
			_size.y = y;
			_dirty = true;
		}
	}
	void Sprite::setSize(const glm::vec2& size)
	{
		setSize(size.x, size.y);
	}
	const glm::ivec2& Sprite::sheetSize() const
	{
		return _sheetSize;
	}
	void Sprite::setSheetSize(const int& x, const int& y)
	{
		if(_sheetSize.x != x || _sheetSize.y != y)
		{
			_sheetSize.x = x;
			_sheetSize.y = y;
			_dirty = true;
		}
	}
	void Sprite::setSheetSize(const glm::ivec2& size)
	{
		setSheetSize(size.x, size.y);
	}
	const int& Sprite::sheetTile() const
	{
		return _sheetTile;
	}
	void Sprite::setSheetTile(const int& tile)
	{
		if(_sheetTile != tile)
		{
			_sheetTile = tile;
			_dirty = true;
		}
	}
	void Sprite::build()
	{
		_dirty = false;

		const glm::vec4 uv = calculateUV(_sheetSize, _sheetTile);

		glm::vec2 A, B, C;
		if(_centeredPosition)
		{
			A = _position - _size * 0.5f;
			B = _position + _size * 0.5f;
			C = _position;
		}
		else
		{
			A = _position;
			B = A + _size;
			C = A + _size * 0.5f;
		}
		const glm::vec2 points[] = {
			rotatePoint({ A.x, B.y }, C, _rotation),
			rotatePoint({ A.x, A.y }, C, _rotation),
			rotatePoint({ B.x, A.y }, C, _rotation),
			rotatePoint({ B.x, B.y }, C, _rotation)
		};

		const GLfloat vdata[] = {
			points[0].x, points[0].y, uv.x, uv.w, 1, 1, 1,
			points[1].x, points[1].y, uv.x, uv.y, 1, 1, 1,
			points[2].x, points[2].y, uv.z, uv.y, 1, 1, 1,
			points[3].x, points[3].y, uv.z, uv.w, 1, 1, 1,
			0
		};
		const GLuint idata[] = {
			0, 1, 2,
			0, 2, 3,
			0
		};

		_vao.bind();

		_vbo.bind(GL_ARRAY_BUFFER);
		glBufferData(GL_ARRAY_BUFFER, 4 * 7 * sizeof(GLfloat), vdata, GL_STATIC_DRAW);

		_ebo.bind(GL_ELEMENT_ARRAY_BUFFER);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), idata, GL_STATIC_DRAW);

		_vao.unbind();
	}
	void Sprite::render()
	{
		if(_dirty)
		{
			build();
		}

		_vao.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
		_vao.unbind();
	}
	glm::vec4 Sprite::calculateUV(const glm::ivec2& sheetSize, const glm::ivec2& tile)
	{
		const glm::vec2 tileSize = 1.0f / glm::vec2(sheetSize);
		return glm::vec4(
			glm::vec2(tile) * tileSize,
			glm::vec2(tile + glm::ivec2(1, 1)) * tileSize
		);
	}
	glm::vec4 Sprite::calculateUV(const glm::ivec2& sheetSize, const int& n)
	{
		const glm::ivec2 tile(
			n % sheetSize.x,
			(n / sheetSize.x) % sheetSize.y
		);
		return calculateUV(sheetSize, tile);
	}
}
