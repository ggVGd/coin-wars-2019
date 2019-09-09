#pragma once
#include "VertexArray.h"
#include "Buffer.h"
#include "Texture.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace Cinnabar
{
	class Sprite
	{
	public:
		Sprite();

		std::shared_ptr<Texture> texture() const;
		void setTexture(std::shared_ptr<Texture>);

		const bool& centeredPosition() const;
		void setCenteredPosition(bool);

		const glm::vec2& position() const;
		void setPosition(const float&, const float&);
		void setPosition(const glm::vec2&);

		const int& zIndex() const;
		void setZIndex(int);

		const float& rotation() const;
		void setRotation(const float&);

		const glm::vec2& size() const;
		void setSize(const float&, const float&);
		void setSize(const glm::vec2&);

		const glm::ivec2& sheetSize() const;
		void setSheetSize(const int&, const int&);
		void setSheetSize(const glm::ivec2&);

		const int& sheetTile() const;
		void setSheetTile(const int&);

		void build();
		void render();

		static glm::vec4 calculateUV(const glm::ivec2& sheetSize, const glm::ivec2& tile);
		static glm::vec4 calculateUV(const glm::ivec2& sheetSize, const int& n);

	private:
		Cinnabar::VertexArray _vao;
		Cinnabar::Buffer _vbo;
		Cinnabar::Buffer _ebo;

		bool _dirty;
		std::shared_ptr<Texture> _texture;
		bool _centeredPosition = false;
		glm::vec2 _position;
		int _zIndex;
		float _rotation = 0.0f;
		glm::vec2 _size;
		glm::ivec2 _sheetSize = {1, 1};
		int _sheetTile = 0;
	};
}
