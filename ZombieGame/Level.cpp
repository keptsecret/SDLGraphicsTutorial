#include "Level.h"

#include <fstream>
#include <iostream>

#include <SkeletonEngine/Errors.h>

#include "SkeletonEngine/ResourceManager.h"

Level::Level(const std::string& file_name)
{
	std::ifstream file;
	file.open(file_name);
	if (file.fail())
	{
		SkeletonEngine::fatalError("Failed to open " + file_name);
	}

	// throw away first string into tmp
	std::string tmp;
	file >> tmp >> num_humans_;

	while (std::getline(file, tmp))
	{
		level_data_.push_back(tmp);
	}

	sprite_batch_.init();
	sprite_batch_.begin();
	glm::vec4 uv_rect{ 0.0f, 0.0f, 1.0f, 1.0f };
	SkeletonEngine::Color white_color{ 255, 255, 255, 255 };

	// render all the tiles
	for (size_t y = 0; y < level_data_.size(); y++)
	{
		for (size_t x = 0; x < level_data_[y].size(); x++)
		{
			// get tile as char
			char tile_type = level_data_[y][x];

			// get dest rect (where to draw tile)
			glm::vec4 dest_rect{x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH};

			// process tile
			switch(tile_type)
			{
			case 'R':
				sprite_batch_.draw(dest_rect,
					uv_rect,
					SkeletonEngine::ResourceManager::getTexture("Textures/red_bricks.png").id,
					0.0f,
					white_color);
				break;
			case 'G':
				sprite_batch_.draw(dest_rect,
					uv_rect,
					SkeletonEngine::ResourceManager::getTexture("Textures/glass.png").id,
					0.0f,
					white_color);
				break;
			case 'L':
				sprite_batch_.draw(dest_rect,
					uv_rect,
					SkeletonEngine::ResourceManager::getTexture("Textures/light_bricks.png").id,
					0.0f,
					white_color);
				break;
			case '@':
				level_data_[y][x] = '.';		// remove to prevent counting as collision part of level
				player_start_pos_.x = x * TILE_WIDTH;
				player_start_pos_.y = y * TILE_WIDTH;
				break;
			case 'Z':
				level_data_[y][x] = '.';		// same here
				zombie_start_pos_.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol %c at (%d, %d)", tile_type, x, y);
			}
		}
	}

	sprite_batch_.end();
}

Level::~Level()
{
}

void Level::draw()
{
	sprite_batch_.renderBatch();
}
