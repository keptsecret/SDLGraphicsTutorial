#pragma once

#include <glm/glm.hpp>

#include "GLTexture.h"
#include "SpriteBatch.h"
#include "Vertex.h"

namespace SkeletonEngine
{
	class Particle2D
	{
	public:
		friend class ParticleBatch2D;

		void update(float delta_time);

	private:
		glm::vec2 position_ = glm::vec2(0.0f);
		glm::vec2 velocity_ = glm::vec2(0.0f);
		ColorRGBA8 color_;
		float life_ = 1.0f;
		bool is_active_ = false;
		float width_ = 0.0f;
	};

	class ParticleBatch2D
	{
	public:
		ParticleBatch2D();
		~ParticleBatch2D();

		void init(int max_particles, float decay_rate, GLTexture texture);
		void update(float delta_time);
		void draw(SpriteBatch* sprite_batch);

		void addParticle(const glm::vec2& pos, const glm::vec2& vel, const ColorRGBA8& color, float width);

	private:
		int findFreeParticle();

		float decay_rate_ = 0.1f;
		Particle2D* particles_ = nullptr;
		int max_particles_ = 0;
		int last_free_particle_ = 0;
		GLTexture texture_;
	};

}

