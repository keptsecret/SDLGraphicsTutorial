#pragma once
#include <string>
#include <vector>

#include <SkeletonEngine/SpriteBatch.h>

const int TILE_WIDTH = 64;

class Level
{
public:
	// load level in constructor
	Level(const std::string& file_name);
	~Level();

	void draw();

private:
	std::vector<std::string> level_data_;
	int num_humans_;
	SkeletonEngine::SpriteBatch sprite_batch_;

	glm::ivec2 player_start_pos_;
	std::vector<glm::ivec2> zombie_start_pos_;
};

