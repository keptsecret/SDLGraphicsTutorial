#include "Sprite.h"

Sprite::Sprite()
{
	vbo_id_ = 0;
}

Sprite::~Sprite()
{
	if (vbo_id_ != 0)
	{
		glDeleteBuffers(1, &vbo_id_);
	}
}

void Sprite::init(float x, float y, float width, float height)
{
	x_ = x;
	y_ = y;
	width_ = width;
	height_ = height;

	if (vbo_id_ == 0)
	{
		glGenBuffers(1, &vbo_id_);
	}

	float vertices[12];

	// first triangle
	vertices[0] = x + width;
	vertices[1] = y + height;

	vertices[2] = x;
	vertices[3] = y + height;

	vertices[4] = x;
	vertices[5] = y;

	// second triangle
	vertices[6] = x;
	vertices[7] = y;

	vertices[8] = x + width;
	vertices[9] = y;

	vertices[10] = x + width;
	vertices[11] = y + height;

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
