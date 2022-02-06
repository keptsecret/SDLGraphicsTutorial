#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <glew/glew.h>

#include "Vertex.h"

namespace SkeletonEngine
{

	enum class GlyphSortType
	{
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	class Glyph
	{
	public:
		Glyph() {}
		Glyph(const glm::vec4& dest_rect, const glm::vec4& uv_rect, GLuint Texture, float Depth, const ColorRGBA8& color) :
			texture(Texture), depth(Depth)
		{
			top_left.color = color;
			top_left.setPosition(dest_rect.x, dest_rect.y + dest_rect.w);
			top_left.setUV(uv_rect.x, uv_rect.y + uv_rect.w);

			bottom_left.color = color;
			bottom_left.setPosition(dest_rect.x, dest_rect.y);
			bottom_left.setUV(uv_rect.x, uv_rect.y);

			bottom_right.color = color;
			bottom_right.setPosition(dest_rect.x + dest_rect.z, dest_rect.y);
			bottom_right.setUV(uv_rect.x + uv_rect.z, uv_rect.y);

			top_right.color = color;
			top_right.setPosition(dest_rect.x + dest_rect.z, dest_rect.y + dest_rect.w);
			top_right.setUV(uv_rect.x + uv_rect.z, uv_rect.y + uv_rect.w);
		}

		GLuint texture;
		float depth;

		Vertex top_left;
		Vertex top_right;
		Vertex bottom_left;
		Vertex bottom_right;
	};

	class RenderBatch
	{
	public:
		RenderBatch(GLuint _offset, GLuint _num_vertices, GLuint _texture) : offset(_offset), num_vertices(_num_vertices), texture(_texture) {};

		GLuint offset;
		GLuint num_vertices;
		GLuint texture;
	};

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void init();

		void begin(GlyphSortType sort_type = GlyphSortType::TEXTURE);
		void end();

		void draw(const glm::vec4& dest_rect, const glm::vec4& uv_rect, GLuint texture, float depth, const ColorRGBA8& color);
		void renderBatch();

	private:
		GLuint vbo_;
		GLuint vao_;

		std::vector<Glyph*> glyph_ptrs_;			// for sorting
		std::vector<Glyph> glyphs_;					// actual glyphs
		GlyphSortType sort_type_;
		std::vector<RenderBatch> render_batches_;

		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();

		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);
	};
}
