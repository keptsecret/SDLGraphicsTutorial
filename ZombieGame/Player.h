#pragma once
#include "Human.h"

class Player : public Human
{
public:
	Player();
	~Player();

	void init(int speed, glm::vec2 pos);
	void update();
};

