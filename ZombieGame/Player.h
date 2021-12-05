#pragma once
#include "Human.h"

#include <SkeletonEngine/InputManager.h>

class Player : public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 pos, SkeletonEngine::InputManager* input_manager);
	void update();

private:
	SkeletonEngine::InputManager* input_manager_;
};

