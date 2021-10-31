#include "MainGame.h"

#include <iostream>

#include <SkeletonEngine/SkeletonEngine.h>
#include <SkeletonEngine/Errors.h>
#include <SkeletonEngine/ImageLoader.h>

#include "SkeletonEngine/ResourceManager.h"

MainGame::MainGame(): screen_width_(1024), screen_height_(768), game_state_(GameState::PLAY), time_(0.0f), max_fps_(60.0f)
{
	camera_.init(screen_width_, screen_height_);
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();

	gameLoop();
}

void MainGame::initSystems()
{
	SkeletonEngine::init();

	window_.create("Game Engine", screen_width_, screen_height_, 0);

	initShaders();

	sprite_batch_.init();
	fps_limiter_.init(max_fps_);
}

void MainGame::initShaders()
{
	color_program_.compileShaders("./Shaders/color_shading.vert", "./Shaders/color_shading.frag");
	color_program_.addAttribute("vertexPosition");
	color_program_.addAttribute("vertexColor");
	color_program_.addAttribute("vertexUV");
	color_program_.linkShaders();
}

void MainGame::gameLoop()
{
	while (game_state_ != GameState::EXIT)
	{
		// for frame time measuring
		fps_limiter_.begin();

		processInput();
		time_ += 0.01f;

		camera_.update();

		for (int i = 0; i < bullets_.size();)
		{
			if (bullets_[i].update() == true)
			{
				bullets_[i] = bullets_.back();
				bullets_.pop_back();
			}
			else
			{
				i++;
			}
		}

		drawGame();

		fps_ = fps_limiter_.end();

		// print only every 10 frames
		static int frame_count = 0;
		frame_count++;
		if (frame_count == 1000)
		{
			std::cout << fps_ << std::endl;
			frame_count = 0;
		}
	}
}

void MainGame::processInput()
{
	SDL_Event event;
	const float CAMERA_SPEED = 5.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			game_state_ = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			input_manager_.setMouseCoords(event.motion.x, event.motion.y);
			break;
		case SDL_KEYDOWN:
			input_manager_.pressKey(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			input_manager_.releaseKey(event.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			input_manager_.pressKey(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			input_manager_.releaseKey(event.button.button);
			break;			
		}
	}

	if (input_manager_.isKeyPressed(SDLK_w))
	{
		camera_.setPosition(camera_.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (input_manager_.isKeyPressed(SDLK_s))
	{
		camera_.setPosition(camera_.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}
	if (input_manager_.isKeyPressed(SDLK_d))
	{
		camera_.setPosition(camera_.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}
	if (input_manager_.isKeyPressed(SDLK_a))
	{
		camera_.setPosition(camera_.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	if (input_manager_.isKeyPressed(SDLK_q))
	{
		camera_.setScale(camera_.getScale() + SCALE_SPEED);
	}
	if (input_manager_.isKeyPressed(SDLK_e))
	{
		camera_.setScale(camera_.getScale() - SCALE_SPEED);
	}

	if (input_manager_.isKeyPressed(SDL_BUTTON_LEFT))
	{
		glm::vec2 mouse_coords = input_manager_.getMouseCoords();
		mouse_coords = camera_.convertScreenToWorldCoords(mouse_coords);
		
		const glm::vec2 player_pos(0.0f);
		glm::vec2 dir = mouse_coords - player_pos;
		dir = glm::normalize(dir);

		bullets_.emplace_back(player_pos, dir, 5.0f, 1000);
	}
}

void MainGame::drawGame()
{
	glClearDepth(1.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	color_program_.use();
	glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, player_texture_.id);		// redundant now that bind is done through sprite
	GLint texture_location = color_program_.getUniformLocation("samplerTex");
	glUniform1i(texture_location, 0);

	// commented out to avoid code optimization when time is not used in shader code
	// GLint time_location = color_program_.getUniformLocation("time");
	// glUniform1f(time_location, time_);

	GLint p_location = color_program_.getUniformLocation("P");
	glm::mat4 camera_matrix = camera_.getCameraMatrix();

	glUniformMatrix4fv(p_location, 1, GL_FALSE, &(camera_matrix[0][0]));

	sprite_batch_.begin();

	glm::vec4 pos(0, 0, 50, 50);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static SkeletonEngine::GLTexture texture = SkeletonEngine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	SkeletonEngine::Color color{255, 255, 255, 255};

	sprite_batch_.draw(pos, uv, texture.id, 0.0f, color);

	for (int i = 0; i < bullets_.size(); i++)
	{
		bullets_[i].draw(sprite_batch_);
	}

	sprite_batch_.end();
	sprite_batch_.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	color_program_.unuse();

	window_.swapBuffer();
}
