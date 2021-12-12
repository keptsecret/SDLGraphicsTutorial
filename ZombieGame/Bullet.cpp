#include "Bullet.h"

#include "Agent.h"
#include "Human.h"
#include "Zombie.h"
#include "Level.h"

#include "SkeletonEngine/ResourceManager.h"

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, float dmg) :
	damage_(dmg),
	position_(pos),
	direction_(dir),
	speed_(speed)
{
	
}

Bullet::~Bullet()
{
	//Empty
}

bool Bullet::update(const std::vector<std::string>& level_data)
{
	position_ += direction_ * speed_;
	return collideWithWorld(level_data);
}

void Bullet::draw(SkeletonEngine::SpriteBatch& sprite_batch)
{
	glm::vec4 dest_rect(position_.x + BULLET_RADIUS, position_.y + BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);
	glm::vec4 uv_rect(0.0f, 0.0f, 1.0f, 1.0f);
	SkeletonEngine::Color color{ 75, 75, 75, 255 };
	sprite_batch.draw(dest_rect, uv_rect, SkeletonEngine::ResourceManager::getTexture("Textures/circle.png").id, 0.0f, color);
}

bool Bullet::collideWithAgent(Agent* agent)
{
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

	// our center
	glm::vec2 center_posA = position_;
	// center of other agent
	glm::vec2 center_posB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 dist_vec = center_posA - center_posB;
	float distance = glm::length(dist_vec);
	float collision_depth = MIN_DISTANCE - distance;

	if (collision_depth > 0)
	{
		return true;
	}
	return false;
}

bool Bullet::collideWithWorld(const std::vector<std::string>& level_data)
{
	glm::ivec2 grid_pos = glm::ivec2{ floor(position_.x / TILE_WIDTH), floor(position_.y / TILE_WIDTH) };

	// if outside world just return true
	if (grid_pos.x < 0 || grid_pos.x >= level_data[0].length() ||
		grid_pos.y < 0 || grid_pos.y >= level_data.size()) return true;

	return level_data[grid_pos.y][grid_pos.x] != '.';
}

