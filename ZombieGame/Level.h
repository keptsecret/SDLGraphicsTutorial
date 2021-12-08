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

	// getters
	glm::vec2 getPlayerStartPos() const { return player_start_pos_; }
	const std::vector<glm::vec2>& getZombieStartPos() const { return zombie_start_pos_; }
	const std::vector<std::string>& getLevelData() const { return level_data_; }
	int getNumHumans() const { return num_humans_; }
	int getWidth() const { return level_data_[0].size(); }
	int getHeight() const { return level_data_.size(); }

private:
	std::vector<std::string> level_data_;
	int num_humans_;
	SkeletonEngine::SpriteBatch sprite_batch_;

	glm::vec2 player_start_pos_;
	std::vector<glm::vec2> zombie_start_pos_;
};

