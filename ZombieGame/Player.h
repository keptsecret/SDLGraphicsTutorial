#pragma once
#include "Human.h"

#include <SkeletonEngine/InputManager.h>
#include <SkeletonEngine/Camera2D.h>

#include "Bullet.h"

class Gun;

class Player : public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 pos, SkeletonEngine::InputManager* input_manager, SkeletonEngine::Camera2D* camera, std::vector<Bullet>* bullets);
	void update(const std::vector<std::string>& level_data,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies);

	void addGun(Gun* gun);

private:
	SkeletonEngine::InputManager* input_manager_;
	SkeletonEngine::Camera2D* camera_;
	std::vector<Bullet>* bullets_;

	std::vector<Gun*> guns_;
	int curr_gun_idx_;
};

