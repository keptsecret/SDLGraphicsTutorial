#pragma once

#include <GL/glew.h>

class Sprite
{
public:
	Sprite();
	~Sprite();

	void init(float x, float y, float width, float height);

	void draw();

private:
	float x_;
	float y_;
	float width_;
	float height_;

	GLuint vbo_id_;
};

