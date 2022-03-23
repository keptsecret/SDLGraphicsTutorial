#pragma once
#include <vector>

namespace SkeletonEngine
{
	class ParticleBatch2D;
	class SpriteBatch;

	class ParticleEngine2D
	{
	public:
		ParticleEngine2D();
		~ParticleEngine2D();

		void update(float delta_time);
		void draw(SpriteBatch* sprite_batch);

		// ParticleEngine2D responsible for deallocating particle batch after adding
		void addParticleBatch(ParticleBatch2D* particle_batch);

	private:
		std::vector<ParticleBatch2D*> batches_;
	};
}
