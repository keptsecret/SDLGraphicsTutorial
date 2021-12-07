#pragma once
#include "Human.h"

#include <SkeletonEngine/InputManager.h>

class Player : public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 pos, SkeletonEngine::InputManager* input_manager);
	void update(const std::vector<std::string>& level_data,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies);

private:
	SkeletonEngine::InputManager* input_manager_;
};

