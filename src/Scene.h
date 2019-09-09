#pragma once
#include "Cinnabar/Sprite.h"
#include <vector>

struct Scene
{
	std::vector<std::shared_ptr<Cinnabar::Sprite>> sprites;

	void removeSprite(std::shared_ptr<Cinnabar::Sprite> sprite);
};
