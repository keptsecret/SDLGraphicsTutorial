#include "Player.h"

#include <SDL/SDL.h>

Player::Player()
{
}

Player::~Player()
{
}


void Player::init(float speed, glm::vec2 pos, SkeletonEngine::InputManager* input_manager)
{
	speed_ = speed;
	position_ = pos;
	color_ = {0, 0, 185, 255};
	input_manager_ = input_manager;
}

void Player::update()
{
	if (input_manager_->isKeyPressed(SDLK_w))
	{
		position_.y += speed_;
	}
	else if (input_manager_->isKeyPressed(SDLK_s))
	{
		position_.y -= speed_;
	}

	if (input_manager_->isKeyPressed(SDLK_d))
	{
		position_.x += speed_;
	}
	else if (input_manager_->isKeyPressed(SDLK_a))
	{
		position_.x -= speed_;
	}
}
