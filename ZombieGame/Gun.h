#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <SkeletonEngine/AudioEngine.h>

#include "Bullet.h"

class Gun
{
public:
	Gun(std::string name, int fire_rate, int bps, float spread, float speed, float dmg, SkeletonEngine::SoundEffect fire_sfx);
	~Gun();

	void update(bool is_mouse_down, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float delta_time);

private:
	std::string name_;
	int fire_rate_;			// in terms of frames
	int bullets_per_shot_;	// how many bullets fired at a time
	float spread_;			// accuracy

	float bullet_speed_;
	float bullet_damage_;
	float frame_counter_;

	SkeletonEngine::SoundEffect fire_sfx_;

	void fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet>& bullets);

};

