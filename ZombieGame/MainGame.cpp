#include "MainGame.h"

#include <iostream>
#include <random>
#include <ctime>

#include <SkeletonEngine/SkeletonEngine.h>
#include <SkeletonEngine/Timing.h>
#include <SkeletonEngine/Errors.h>

#include "Zombie.h"

const float HUMAN_SPEED = 2.0f;
const float ZOMBIE_SPEED = 2.5f;
const float PLAYER_SPEED = 5.0f;

MainGame::MainGame(): height_(768), width_(1024), player_(nullptr), game_state_(GameState::PLAY), fps_(0)
{
}

MainGame::~MainGame()
{
	for (const Level* l : levels_)
	{
		delete l;
	}
}

void MainGame::run()
{
	initSystems();
	initLevel();
	gameLoop();
}

void MainGame::initSystems()
{
	SkeletonEngine::init();

	window_.create("Zombie Game", width_, height_, 0);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	initShaders();

	sprite_batch_.init();
	camera_.init(width_, height_);
}

void MainGame::initShaders()
{
	texture_program_.compileShaders("Shaders/tex_shading.vert", "Shaders/tex_shading.frag");
	texture_program_.addAttribute("vertexPosition");
	texture_program_.addAttribute("vertexColor");
	texture_program_.addAttribute("vertexUV");
	texture_program_.linkShaders();
}

void MainGame::initLevel()
{
	// Level 1
	levels_.push_back(new Level("Levels/level1.txt"));
	current_level_ = 0;

	player_ = new Player();
	player_->init(PLAYER_SPEED, levels_[current_level_]->getPlayerStartPos(), &input_manager_);

	humans_.push_back(player_);

	std::mt19937 random_engine;
	random_engine.seed(time(nullptr));
	std::uniform_int_distribution<int> rand_X{ 1, levels_[current_level_]->getWidth() - 1 };
	std::uniform_int_distribution<int> rand_Y{ 1, levels_[current_level_]->getHeight() - 1 };

	// add humans
	for (int i = 0; i < levels_[current_level_]->getNumHumans(); i++)
	{
		humans_.push_back(new Human());
		glm::vec2 pos{ rand_X(random_engine) * TILE_WIDTH, rand_Y(random_engine) * TILE_WIDTH };
		humans_.back()->init(HUMAN_SPEED, pos);
	}

	// add zombie(s)
	const std::vector<glm::vec2>& zombie_pos = levels_[current_level_]->getZombieStartPos();
	for (int i = 0; i < zombie_pos.size(); i++)
	{
		zombies_.push_back(new Zombie());
		zombies_.back()->init(ZOMBIE_SPEED, zombie_pos[i]);
	}
}

void MainGame::updateAgents()
{
	// update humans
	for (Human* h : humans_)
	{
		h->update(levels_[current_level_]->getLevelData(), humans_, zombies_);
	}

	// update zombies
	for (Zombie* z : zombies_)
	{
		z->update(levels_[current_level_]->getLevelData(), humans_, zombies_);
	}

	// Update zombie collisions
	for (int i = 0; i < zombies_.size(); i++)
	{
		// collide with other zombies
		for (int j = i + 1; j < zombies_.size(); j++)
		{
			zombies_[i]->collideWithAgent(zombies_[j]);
		}

		// collide with humans (player is at human[0])
		for (int j = 1; j < humans_.size(); j++)
		{
			if (zombies_[i]->collideWithAgent(humans_[j]))
			{
				// add new zombie
				zombies_.push_back(new Zombie());
				zombies_.back()->init(ZOMBIE_SPEED, humans_[j]->getPosition());
				// delete human
				delete humans_[j];
				humans_[j] = humans_.back();
				humans_.pop_back();
			}
		}

		// collide with player
		if (zombies_[i]->collideWithAgent(player_))
		{
			SkeletonEngine::fatalError("YOU LOSE");
		}
	}

	// Update human collisions
	for (int i = 0; i < humans_.size(); i++)
	{
		for (int j = i + 1; j < humans_.size(); j++)
		{
			humans_[i]->collideWithAgent(humans_[j]);
		}
	}

}


void MainGame::gameLoop()
{
	SkeletonEngine::FpsLimiter fps_limiter;
	fps_limiter.setMaxFPS(60.0f);

	while (game_state_ == GameState::PLAY)
	{
		fps_limiter.begin();

		processInput();
		updateAgents();
		camera_.setPosition(player_->getPosition());
		camera_.update();
		drawGame();

		fps_ = fps_limiter.end();
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			// Need to exit game here
			break;
		case SDL_MOUSEMOTION:
			input_manager_.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			input_manager_.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			input_manager_.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			input_manager_.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			input_manager_.releaseKey(evnt.button.button);
			break;
		}
	}
}

void MainGame::drawGame()
{
	// Set base depth to 1.0 and clear color and depth buffer
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	texture_program_.use();

	// drawing code
	glActiveTexture(GL_TEXTURE0);

	// make sure shader uses texture 0
	GLint texture_uniform = texture_program_.getUniformLocation("samplerTex");
	glUniform1i(texture_uniform, 0);

	glm::mat4 projection_matrix = camera_.getCameraMatrix();
	GLint p_uniform = texture_program_.getUniformLocation("P");
	glUniformMatrix4fv(p_uniform, 1, GL_FALSE, &projection_matrix[0][0]);

	// draw the level
	levels_[current_level_]->draw();

	sprite_batch_.begin();

	// draw the humans
	for (Human* h : humans_)
	{
		h->draw(sprite_batch_);
	}

	// draw the zombies
	for (Zombie* z : zombies_)
	{
		z->draw(sprite_batch_);
	}

	sprite_batch_.end();
	sprite_batch_.renderBatch();

	texture_program_.unuse();

	// swap buffer to draw to game
	window_.swapBuffer();
}
