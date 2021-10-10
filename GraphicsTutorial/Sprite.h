#pragma once

#include <string>
#include <GL/glew.h>

#include "GLTexture.h"

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

