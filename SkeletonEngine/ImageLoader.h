#pragma once

#include <string>

#include "GLTexture.h"

namespace SkeletonEngine
{

	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string file_path);
	};

}
