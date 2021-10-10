#pragma once

#include <string>

#include "GLTexture.h"

class ImageLoader
{
public:
	static GLTexture loadPNG(std::string file_path);
};

