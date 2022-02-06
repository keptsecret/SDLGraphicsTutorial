#include "SpriteBatch.h"

#include <algorithm>

namespace SkeletonEngine
{

	SpriteBatch::SpriteBatch(): vbo_(0), vao_(0)
	{
	}

	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::init()
	{
		createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sort_type)
	{
		sort_type_ = sort_type;
		render_batches_.clear();
		glyphs_.clear();
	}

	void SpriteBatch::end()
	{
		// set up pointers for sorting
		glyph_ptrs_.resize(glyphs_.size());
		for (int i = 0; i < glyphs_.size(); i++)
		{
			glyph_ptrs_[i] = &glyphs_[i];
		}

		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& dest_rect, const glm::vec4& uv_rect, GLuint texture, float depth, const ColorRGBA8& color)
	{
		glyphs_.emplace_back(dest_rect, uv_rect, texture, depth, color);
	}

	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(vao_);

		for (RenderBatch rb : render_batches_)
		{
			glBindTexture(GL_TEXTURE_2D, rb.texture);

			glDrawArrays(GL_TRIANGLES, rb.offset, rb.num_vertices);
		}

		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches()
	{
		std::vector<Vertex> vertices;
		vertices.resize(glyphs_.size() * 6);

		if (glyphs_.empty())
		{
			return;
		}

		int curr_vertex = 0;
		int offset = 0;
		render_batches_.emplace_back(0, 6, glyph_ptrs_[0]->texture);
		vertices[curr_vertex++] = glyph_ptrs_[0]->top_left;
		vertices[curr_vertex++] = glyph_ptrs_[0]->bottom_left;
		vertices[curr_vertex++] = glyph_ptrs_[0]->bottom_right;
		vertices[curr_vertex++] = glyph_ptrs_[0]->bottom_right;
		vertices[curr_vertex++] = glyph_ptrs_[0]->top_right;
		vertices[curr_vertex++] = glyph_ptrs_[0]->top_left;
		offset += 6;

		for (int curr_glyph = 1; curr_glyph < glyph_ptrs_.size(); curr_glyph++)
		{
			if (glyph_ptrs_[curr_glyph]->texture != glyph_ptrs_[curr_glyph - 1]->texture)
			{
				render_batches_.emplace_back(offset, 6, glyph_ptrs_[curr_glyph]->texture);
			}
			else
			{
				render_batches_.back().num_vertices += 6;
			}
			
			vertices[curr_vertex++] = glyph_ptrs_[curr_glyph]->top_left;
			vertices[curr_vertex++] = glyph_ptrs_[curr_glyph]->bottom_left;
			vertices[curr_vertex++] = glyph_ptrs_[curr_glyph]->bottom_right;
			vertices[curr_vertex++] = glyph_ptrs_[curr_glyph]->bottom_right;
			vertices[curr_vertex++] = glyph_ptrs_[curr_glyph]->top_right;
			vertices[curr_vertex++] = glyph_ptrs_[curr_glyph]->top_left;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		// orphan the buffer (throw current stored data away)
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		// upload the data
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::createVertexArray()
	{
		if (!vao_)
		{
			glGenVertexArrays(1, &vao_);
		}
		glBindVertexArray(vao_);

		if (!vbo_)
		{
			glGenBuffers(1, &vbo_);
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		// color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		// UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}

	void SpriteBatch::sortGlyphs()
	{
		switch (sort_type_)
		{
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(glyph_ptrs_.begin(), glyph_ptrs_.end(), compareBackToFront);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(glyph_ptrs_.begin(), glyph_ptrs_.end(), compareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(glyph_ptrs_.begin(), glyph_ptrs_.end(), compareTexture);
			break;
		}
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b)
	{
		return a->depth < b->depth;
	}

	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b)
	{
		return a->depth > b->depth;
	}

	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b)
	{
		return a->texture < b->texture;
	}
}
