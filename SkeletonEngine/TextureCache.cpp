#include "TextureCache.h"

#include <iostream>

#include "ImageLoader.h"

namespace SkeletonEngine
{

	TextureCache::TextureCache()
	{
	}

	TextureCache::~TextureCache()
	{
	}

	GLTexture TextureCache::getTexture(const std::string& texture_path)
	{
		// look up texture if in map
		auto mit = texture_map_.find(texture_path);	// of type std::map<std::string, GLTexture>::iterator

		// check if texture is in map
		if (mit == texture_map_.end())
		{
			// load texture and put into map
			GLTexture new_texture = ImageLoader::loadPNG(texture_path);
			texture_map_.insert(std::make_pair(texture_path, new_texture));

			std::cout << "Loaded cached texture!" << std::endl;
			return new_texture;
		}

		std::cout << "Loaded texture!" << std::endl;
		return mit->second;
	}

}
