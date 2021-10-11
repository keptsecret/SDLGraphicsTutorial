#pragma once

#include <map>
#include <string>

#include "GLTexture.h"

namespace SkeletonEngine
{

	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		GLTexture getTexture(const std::string& texture_path);

	private:
		std::map<std::string, GLTexture> texture_map_;
	};

}
