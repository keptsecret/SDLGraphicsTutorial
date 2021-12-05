#pragma once
#include <glm/glm.hpp>

class Agent
{
public:
	Agent();
	virtual ~Agent();

protected:
	glm::vec2 position_;
	float speed_;
};

