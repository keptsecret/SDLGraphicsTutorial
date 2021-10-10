#include "ResourceManager.h"

TextureCache ResourceManager::texture_cache_;

GLTexture ResourceManager::getTexture(const std::string& texture_path)
{
	return texture_cache_.getTexture(texture_path);
}
