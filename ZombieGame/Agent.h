#pragma once
#include <glm/glm.hpp>

#include <SkeletonEngine/SpriteBatch.h>

const int AGENT_WIDTH = 60;

class Agent
{
public:
	Agent();
	virtual ~Agent();

	virtual void update() = 0;

	void draw(SkeletonEngine::SpriteBatch& sprite_batch);

	glm::vec2 getPosition() const { return position_; }

protected:
	glm::vec2 position_;
	SkeletonEngine::Color color_;
	float speed_;
};

