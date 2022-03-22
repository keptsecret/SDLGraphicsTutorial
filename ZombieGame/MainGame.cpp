#include "MainGame.h"

#include <iostream>
#include <random>
#include <ctime>

#include <SkeletonEngine/SkeletonEngine.h>
#include <SkeletonEngine/Timing.h>
#include <SkeletonEngine/SkellyErrors.h>

#include "Zombie.h"
#include "Gun.h"

const float HUMAN_SPEED = 2.0f;
const float ZOMBIE_SPEED = 2.5f;
const float PLAYER_SPEED = 5.0f;

MainGame::MainGame(): height_(768), width_(1024), player_(nullptr), game_state_(GameState::PLAY), fps_(0), num_humans_kiled_(0), num_zombies_killed_(0)
{
}

MainGame::~MainGame()
{
	for (const Level* l : levels_)
	{
		delete l;
	}

	for (Human* h : humans_)
	{
		delete h;
	}

	for (Zombie* z : zombies_)
	{
		delete z;
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
	hud_sprite_batch_.init();

	// initialize sprite font, has to be after SDL and OpenGL
	sprite_font_ = new SkeletonEngine::SpriteFont("Fonts/KGHAPPYShadows.ttf", 32);

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
	player_->init(PLAYER_SPEED, levels_[current_level_]->getPlayerStartPos(), &input_manager_, &camera_, &bullets_);

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

	// set up player's guns
	const float PI = 3.14159265359f;
	const float BULLET_SPEED = 20.0f;
	player_->addGun(new Gun("Magnum", 10, 1, 5.0f / 180.0f * PI, BULLET_SPEED, 30.0f));
	player_->addGun(new Gun("Shotgun", 30, 12, 20.0f / 180.0f * PI, BULLET_SPEED, 4.0f));
	player_->addGun(new Gun("MP-5", 2, 1, 10.0f / 180.0f * PI, BULLET_SPEED, 20.0f));
}

void MainGame::updateAgents(float delta_time)
{
	// update humans (including player)
	for (Human* h : humans_)
	{
		h->update(levels_[current_level_]->getLevelData(), humans_, zombies_, delta_time);
	}

	// update zombies
	for (Zombie* z : zombies_)
	{
		z->update(levels_[current_level_]->getLevelData(), humans_, zombies_, delta_time);
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

void MainGame::updateBullets(float delta_time)
{
	// level collision
	for (int i = 0; i < bullets_.size();)
	{
		// if update returns true, then bullet collided with wall
		if (bullets_[i].update(levels_[current_level_]->getLevelData(), delta_time))
		{
			bullets_[i] = bullets_.back();
			bullets_.pop_back();
		}
		else i++;
	}

	bool was_bullet_removed;

	// agent collision
	for (int i = 0; i < bullets_.size(); i++)
	{
		was_bullet_removed = false;
		// loop zombies
		for (int j = 0; j < zombies_.size();)
		{
			if (bullets_[i].collideWithAgent(zombies_[j]))
			{
				// damage zombie and kill if no health
				if (zombies_[j]->applyDamage(bullets_[i].getDamage()))
				{
					// remove if zombie died
					delete zombies_[j];
					zombies_[j] = zombies_.back();
					zombies_.pop_back();
					num_zombies_killed_++;
				}
				else j++;

				bullets_[i] = bullets_.back();
				bullets_.pop_back();
				was_bullet_removed = true;
				i--;							// make sure not skipping bullet
				break;							// since bullet died, no need to loop through any more
			}
			else j++;
		}

		// loop humans
		if (!was_bullet_removed)
		{
			for (int j = 1; j < humans_.size();)
			{
				if (bullets_[i].collideWithAgent(humans_[j]))
				{
					// damage zombie and kill if no health
					if (humans_[j]->applyDamage(bullets_[i].getDamage()))
					{
						// remove if zombie died
						delete humans_[j];
						humans_[j] = humans_.back();
						humans_.pop_back();
						num_humans_kiled_++;
					}
					else j++;

					bullets_[i] = bullets_.back();
					bullets_.pop_back();
					i--;							// make sure not skipping bullet
					break;							// since bullet died, no need to loop through any more
				}
				else j++;
			}
		}
	}
}

void MainGame::checkVictory()
{
	// TODO: support for multiple levels
	// all zombies dead = win
	if (zombies_.empty())
	{
		std::printf("*** You win! ***\nYou killed %d humans and %d zombies. There are %d/%d humans remaining.",
			num_humans_kiled_, num_zombies_killed_, humans_.size() - 1, levels_[current_level_]->getNumHumans());
		SkeletonEngine::fatalError("");
	}
}

void MainGame::gameLoop()
{
	const float DESIRED_FPS = 60.0f;
	const int MAX_PHYSICS_STEPS = 6;

	SkeletonEngine::FpsLimiter fps_limiter;
	fps_limiter.setMaxFPS(DESIRED_FPS);

	const float CAMERA_SCALE = 0.5f;
	camera_.setScale(CAMERA_SCALE);

	const float MS_PER_SEC = 1000.0f;
	const float DESIRED_FRAMETIME = MS_PER_SEC / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;

	float prev_ticks = SDL_GetTicks();

	while (game_state_ == GameState::PLAY)
	{
		fps_limiter.begin();
		float new_ticks = SDL_GetTicks();
		float frame_time = new_ticks - prev_ticks;
		prev_ticks = new_ticks;
		float total_delta_time = frame_time / DESIRED_FRAMETIME;

		checkVictory();

		input_manager_.update();
		processInput();

		int i = 0;
		while(total_delta_time > 0.0f && i < MAX_PHYSICS_STEPS)
		{
			float delta_time = std::min(total_delta_time, MAX_DELTA_TIME);
			updateAgents(delta_time);
			updateBullets(delta_time);
			total_delta_time -= delta_time;
			i++;
		}
		
		camera_.setPosition(player_->getPosition());
		camera_.update();
		drawGame();

		fps_ = fps_limiter.end();
		std::cout << fps_ << std::endl;
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
			game_state_ = GameState::EXIT;
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

	const glm::vec2 agent_dim(AGENT_RADIUS * 2.0f);

	// draw the humans
	for (Human* h : humans_)
	{
		if (camera_.isboxInView(h->getPosition(), agent_dim))
		{
			h->draw(sprite_batch_);
		}
	}

	// draw the zombies
	for (Zombie* z : zombies_)
	{
		if (camera_.isboxInView(z->getPosition(), agent_dim))
		{
			z->draw(sprite_batch_);
		}
	}

	// draw the bullets
	for (Bullet b : bullets_)
	{
		b.draw(sprite_batch_);
	}

	sprite_batch_.end();
	sprite_batch_.renderBatch();

	drawHud();

	texture_program_.unuse();

	// swap buffer to draw to game
	window_.swapBuffer();
}

void MainGame::drawHud()
{
	/*
	 * TODO: fonts are kind of broken, needs revisiting
	 */
	char buf[256];

	hud_sprite_batch_.begin();

	sprintf_s(buf, "No. of Humans Left: %d", humans_.size());

	sprite_font_->draw(hud_sprite_batch_, buf, glm::vec2(300, 300),
		glm::vec2(4.0f), 0.0f, SkeletonEngine::ColorRGBA8(255, 255, 255, 255));

	hud_sprite_batch_.end();
	hud_sprite_batch_.renderBatch();
}
