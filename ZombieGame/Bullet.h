#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include <SkeletonEngine/SpriteBatch.h>

class Agent;
class Human;
class Zombie;

const int BULLET_RADIUS = 5;

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, float dmg);
	~Bullet();

	// when update returns true, delete bullet
	bool update(const std::vector<std::string>& level_data);
	void draw(SkeletonEngine::SpriteBatch& sprite_batch);

	bool collideWithAgent(Agent* agent);

	float getDamage() const { return damage_; }

private:
	bool collideWithWorld(const std::vector<std::string>& level_data);

	float damage_;
	glm::vec2 position_;
	glm::vec2 direction_;
	float speed_;
};

