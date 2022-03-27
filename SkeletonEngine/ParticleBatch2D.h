#pragma once

#include <functional>
#include <glm/glm.hpp>

#include "GLTexture.h"
#include "SpriteBatch.h"
#include "Vertex.h"

namespace SkeletonEngine
{
	class Particle2D
	{
	public:
		glm::vec2 position = glm::vec2(0.0f);
		glm::vec2 velocity = glm::vec2(0.0f);
		ColorRGBA8 color;
		float life = 1.0f;
		bool is_active = false;
		float width = 0.0f;
	};

	inline void defaultParticleUpdate(Particle2D& particle, float delta_time)
	{
		particle.position += particle.velocity * delta_time;
	}

	class ParticleBatch2D
	{
	public:
		ParticleBatch2D();
		~ParticleBatch2D();

		void init(int max_particles, float decay_rate, GLTexture texture, std::function<void(Particle2D&, float)> update_func = defaultParticleUpdate);
		void update(float delta_time);
		void draw(SpriteBatch* sprite_batch);

		void addParticle(const glm::vec2& pos, const glm::vec2& vel, const ColorRGBA8& color, float width);

	private:
		int findFreeParticle();

		std::function<void(Particle2D&, float)> update_func_;
		float decay_rate_ = 0.1f;
		Particle2D* particles_ = nullptr;
		int max_particles_ = 0;
		int last_free_particle_ = 0;
		GLTexture texture_;
	};

}

