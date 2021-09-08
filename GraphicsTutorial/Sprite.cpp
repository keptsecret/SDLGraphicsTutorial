#include "Sprite.h"

#include <cstddef>

#include "Vertex.h"

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

	Vertex vertices[6];

	// first triangle
	vertices[0].position.x = x + width;
	vertices[0].position.y = y + height;

	vertices[1].position.x = x;
	vertices[1].position.y = y + height;

	vertices[2].position.x = x;
	vertices[2].position.y = y;

	// second triangle
	vertices[3].position.x = x;
	vertices[3].position.y = y;

	vertices[4].position.x = x + width;
	vertices[4].position.y = y;

	vertices[5].position.x = x + width;
	vertices[5].position.y = y + height;

	for (Vertex& vertex : vertices)
	{
		vertex.color.r = 255;
		vertex.color.g = 0;
		vertex.color.b = 255;
		vertex.color.a = 255;
	}

	vertices[1].color.r = 0;
	vertices[1].color.g = 255;
	vertices[1].color.b = 0;
	vertices[1].color.a = 255;

	vertices[4].color.r = 0;
	vertices[4].color.g = 0;
	vertices[4].color.b = 255;
	vertices[4].color.a = 255;

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);

	glEnableVertexAttribArray(0);
	// position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	// color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
