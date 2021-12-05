#include "Agent.h"

#include <SkeletonEngine/ResourceManager.h>

Agent::Agent()
{
}

Agent::~Agent()
{
}

void Agent::draw(SkeletonEngine::SpriteBatch& sprite_batch)
{
	static int texture_id = SkeletonEngine::ResourceManager::getTexture("Textures/circle.png").id;
	const glm::vec4 uv_rect{ 0.0f, 0.0f, 1.0f, 1.0f };
	glm::vec4 dest_rect{position_.x, position_.y, AGENT_WIDTH, AGENT_WIDTH};

	sprite_batch.draw(dest_rect, uv_rect, texture_id, 0.0f, color_);
}
