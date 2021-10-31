#pragma once

#include <glm.hpp>

#include <SkeletonEngine/SpriteBatch.h>

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifetime);
	~Bullet();

	void draw(SkeletonEngine::SpriteBatch& sprite_batch);
	// returns true when out of life
	bool update();

private:
	float speed_;
	glm::vec2 direction_;
	glm::vec2 position_;
	int lifetime_;
};

