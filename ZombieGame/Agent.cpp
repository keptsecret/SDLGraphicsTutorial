#include "Agent.h"

#include <algorithm>
#include <SkeletonEngine/ResourceManager.h>

#include "Level.h"

Agent::Agent()
{
}

Agent::~Agent()
{
}

void Agent::collideWithLevel(const std::vector<std::string>& level_data)
{
	std::vector<glm::vec2> collide_tile_position;

	// check all 4 corners
	checkTilePosition(level_data, collide_tile_position, position_.x, position_.y);
	checkTilePosition(level_data, collide_tile_position, position_.x + AGENT_WIDTH, position_.y);
	checkTilePosition(level_data, collide_tile_position, position_.x, position_.y + AGENT_WIDTH);
	checkTilePosition(level_data, collide_tile_position, position_.x + AGENT_WIDTH, position_.y + AGENT_WIDTH);

	// do collision
	for (const glm::vec2 pos : collide_tile_position)
	{
		collideWithTile(pos);
	}
}

void Agent::draw(SkeletonEngine::SpriteBatch& sprite_batch)
{
	static int texture_id = SkeletonEngine::ResourceManager::getTexture("Textures/circle.png").id;
	const glm::vec4 uv_rect{ 0.0f, 0.0f, 1.0f, 1.0f };
	glm::vec4 dest_rect{position_.x, position_.y, AGENT_WIDTH, AGENT_WIDTH};

	sprite_batch.draw(dest_rect, uv_rect, texture_id, 0.0f, color_);
}

void Agent::checkTilePosition(const std::vector<std::string>& level_data, std::vector<glm::vec2>& collide_tile_pos, float x, float y)
{
	glm::vec2 corner_pos1 = glm::vec2(floor(x / float(TILE_WIDTH)),	floor(y / float(TILE_WIDTH)));

	if (level_data[corner_pos1.y][corner_pos1.x] != '.')
	{
		collide_tile_pos.push_back(corner_pos1 * float(TILE_WIDTH) + glm::vec2(float(TILE_WIDTH) / 2.0f));
	}
}

void Agent::collideWithTile(glm::vec2 tile_pos)
{
	const float AGENT_RADIUS = static_cast<float>(AGENT_WIDTH) / 2.0f;
	const float TILE_RADIUS = static_cast<float>(TILE_WIDTH) / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	glm::vec2 player_center_pos = position_ + glm::vec2(AGENT_RADIUS);
	glm::vec2 dist_vec = player_center_pos - tile_pos;					// how far agent from tile center

	float xdepth = MIN_DISTANCE - abs(dist_vec.x);
	float ydepth = MIN_DISTANCE - abs(dist_vec.y);

	if (xdepth > 0 || ydepth > 0)
	{
		if (std::max(xdepth, 0.0f) < std::max(ydepth, 0.0f))
		{
			if (dist_vec.x < 0)
			{
				position_.x -= xdepth;
			}
			else
			{
				position_.x += xdepth;
			}
		}
		else
		{
			if (dist_vec.y < 0)
			{
				position_.y -= ydepth;
			}
			else
			{
				position_.y += ydepth;
			}
		}
	}
}
