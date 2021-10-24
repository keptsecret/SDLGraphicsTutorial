#include "Sprite.h"

#include <cstddef>

#include "Vertex.h"
#include "ResourceManager.h"

namespace SkeletonEngine
{

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

	void Sprite::init(float x, float y, float width, float height, std::string texture_path)
	{
		x_ = x;
		y_ = y;
		width_ = width;
		height_ = height;
		texture_ = ResourceManager::getTexture(texture_path);

		if (vbo_id_ == 0)
		{
			glGenBuffers(1, &vbo_id_);
		}

		Vertex vertices[6];

		// first triangle
		vertices[0].setPosition(x + width, y + height);
		vertices[0].setUV(1.0f, 1.0f);

		vertices[1].setPosition(x, y + height);
		vertices[1].setUV(0.0f, 1.0f);

		vertices[2].setPosition(x, y);
		vertices[2].setUV(0.0f, 0.0f);

		// second triangle
		vertices[3].setPosition(x, y);
		vertices[3].setUV(0.0f, 0.0f);

		vertices[4].setPosition(x + width, y);
		vertices[4].setUV(1.0f, 0.0f);

		vertices[5].setPosition(x + width, y + height);
		vertices[5].setUV(1.0f, 1.0f);

		for (Vertex& vertex : vertices)
		{
			vertex.setColor(255, 0, 255, 255);
		}

		//change some colors
		vertices[1].setColor(0, 255, 0, 255);

		vertices[4].setColor(0, 0, 255, 255);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Sprite::draw()
	{
		glBindTexture(GL_TEXTURE_2D, texture_.id);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		// color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		// UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
