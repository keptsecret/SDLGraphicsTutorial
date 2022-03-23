#include "ParticleBatch2D.h"

namespace SkeletonEngine
{
	void Particle2D::update(float delta_time)
	{
		position_ += velocity_ * delta_time;
	}

	ParticleBatch2D::ParticleBatch2D()
	{
	}

	ParticleBatch2D::~ParticleBatch2D()
	{
		delete[] particles_;
	}

	void ParticleBatch2D::init(int max_particles, float decay_rate, GLTexture texture)
	{
		max_particles_ = max_particles;
		particles_ = new Particle2D[max_particles_];
		decay_rate_ = decay_rate;
		texture_ = texture;
	}

	void ParticleBatch2D::update(float delta_time)
	{
		for (size_t i = 0; i < max_particles_; i++)
		{
			if (particles_[i].life_ > 0.0f)
			{
				particles_[i].update(delta_time);
				particles_[i].life_ -= decay_rate_ * delta_time;
			}
		}
	}

	void ParticleBatch2D::draw(SpriteBatch* sprite_batch)
	{
		glm::vec4 uv_rect(0.0f, 0.0f, 1.0f, 1.0f);
		for (size_t i = 0; i < max_particles_; i++)
		{
			auto& p = particles_[i];
			if (p.life_ > 0.0f)
			{
				glm::vec4 dest_rect(p.position_.x, p.position_.y, p.width_, p.width_);
				sprite_batch->draw(dest_rect, uv_rect, texture_.id, 0.0f, p.color_);
			}
		}
	}

	void ParticleBatch2D::addParticle(const glm::vec2& pos, const glm::vec2& vel, const ColorRGBA8& color, float width)
	{
		int particle_idx = findFreeParticle();
		auto& p = particles_[particle_idx];

		p.life_ = 1.0f;
		p.position_ = pos;
		p.velocity_ = vel;
		p.color_ = color;
		p.width_ = width;
	}

	int ParticleBatch2D::findFreeParticle()
	{
		for (int i = last_free_particle_; i < max_particles_; i++)
		{
			if (particles_[i].life_ <= 0.0f)
			{
				last_free_particle_ = i;
				return i;
			}
		}

		for (int i = 0; i < last_free_particle_; i++)
		{
			if (particles_[i].life_ <= 0.0f)
			{
				last_free_particle_ = i;
				return i;
			}
		}

		// no particles are free, overwrite first particle
		return 0;
	}
}
