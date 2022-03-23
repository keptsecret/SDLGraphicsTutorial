#include "ParticleEngine2D.h"

#include "ParticleBatch2D.h"
#include "SpriteBatch.h"

namespace SkeletonEngine
{
	ParticleEngine2D::ParticleEngine2D()
	{
	}

	ParticleEngine2D::~ParticleEngine2D()
	{
		for (auto& b : batches_)
		{
			delete b;
		}
	}

	void ParticleEngine2D::update(float delta_time)
	{
		for (auto& b : batches_)
		{
			b->update(delta_time);
		}
	}

	void ParticleEngine2D::draw(SpriteBatch* sprite_batch)
	{
		for (auto& b : batches_)
		{
			sprite_batch->begin();

			b->draw(sprite_batch);

			sprite_batch->end();
			sprite_batch->renderBatch();
		}
	}

	void ParticleEngine2D::addParticleBatch(ParticleBatch2D* particle_batch)
	{
		batches_.push_back(particle_batch);
	}
}
