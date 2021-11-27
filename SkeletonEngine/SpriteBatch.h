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

	struct Glyph
	{
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

		void draw(const glm::vec4& dest_rect, const glm::vec4& uv_rect, GLuint texture, float depth, const Color& color);
		void renderBatch();

	private:
		GLuint vbo_;
		GLuint vao_;

		std::vector<Glyph*> glyphs_;
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
