#include "Scene.h"
#include <algorithm>

void Scene::removeSprite(std::shared_ptr<Cinnabar::Sprite> sprite)
{
	sprites.erase(std::remove(sprites.begin(), sprites.end(), sprite), sprites.end());
}
