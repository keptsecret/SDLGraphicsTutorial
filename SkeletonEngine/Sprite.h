#pragma once

#include <string>
#include <glew/glew.h>

#include "GLTexture.h"

namespace SkeletonEngine
{

	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string texture_path);

		void draw();

	private:
		float x_;
		float y_;
		float width_;
		float height_;

		GLuint vbo_id_;
		GLTexture texture_;
	};

}
