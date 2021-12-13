#include "Gun.h"

#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

Gun::Gun(std::string name, int fire_rate, int bps, float spread, float speed, float dmg) :
	name_(name),
	fire_rate_(fire_rate),
	bullets_per_shot_(bps),
	spread_(spread),
	bullet_speed_(speed),
	bullet_damage_(dmg),
	frame_counter_(0)
{
	//Empty
}

Gun::~Gun()
{
	//Empty
}

void Gun::update(bool is_mouse_down, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets)
{
	frame_counter_++;
	if (frame_counter_ > fire_rate_ && is_mouse_down)
	{
		fire(direction, position, bullets);
		frame_counter_ = 0;
	}
}

void Gun::fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet>& bullets)
{
	static std::mt19937 random_engine(time(nullptr));
	std::uniform_real_distribution<float> rand_rotate(-spread_, spread_);

	for (int i = 0; i < bullets_per_shot_; i++)
	{
		bullets.emplace_back(position, glm::rotate(direction, rand_rotate(random_engine)), bullet_speed_, bullet_damage_);
	}
}

