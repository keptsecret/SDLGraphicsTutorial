#pragma once

#include <string>

#include "GLTexture.h"

class ImageLoader
{
public:
	GLTexture loadPNG(std::string file_path);
};

