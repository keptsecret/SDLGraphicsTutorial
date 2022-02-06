#pragma once
#include <string>
#include <glm/glm.hpp>

#include <SkeletonEngine/SpriteBatch.h>

const int AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

// forward declaration to prevent circular includes
class Zombie;
class Human;

class Agent
{
public:
	Agent();
	virtual ~Agent();

	virtual void update(const std::vector<std::string>& level_data,
						std::vector<Human*>& humans,
						std::vector<Zombie*>& zombies,
						float delta_time) = 0;

	bool collideWithLevel(const std::vector<std::string>& level_data);
	bool collideWithAgent(Agent* agent);
	bool applyDamage(float dmg);

	void draw(SkeletonEngine::SpriteBatch& sprite_batch);

	glm::vec2 getPosition() const { return position_; }

protected:
	void checkTilePosition(const std::vector<std::string>& level_data, std::vector<glm::vec2>& collide_tile_pos, float x, float y);
	void collideWithTile(glm::vec2 tile_pos);

	glm::vec2 position_;
	SkeletonEngine::ColorRGBA8 color_;
	float speed_;
	float health_;
};

