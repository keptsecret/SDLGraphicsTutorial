#include "MainGame.h"

#include <iostream>

#include <SkeletonEngine/SkeletonEngine.h>
#include <SkeletonEngine/Errors.h>
#include <SkeletonEngine/ImageLoader.h>

MainGame::MainGame(): screen_width_(1024), screen_height_(768), game_state_(GameState::PLAY), time_(0.0f), max_fps_(60.0f)
{
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();

	// Initialize a couple of sprites
	sprites_.push_back(new SkeletonEngine::Sprite());
	sprites_.back()->init(-1.0, -1.0, 1.0, 1.0, "./Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	sprites_.push_back(new SkeletonEngine::Sprite());
	sprites_.back()->init(0.0, -1.0, 1.0, 1.0, "./Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	// sprite_.init(-1.0, -1.0, 2.0, 2.0, "./Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	// player_texture_ = ImageLoader::loadPNG("./Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	gameLoop();
}

void MainGame::initSystems()
{
	SkeletonEngine::init();

	window_.create("Game Engine", screen_width_, screen_height_, 0);

	initShaders();
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
		float start_ticks = SDL_GetTicks();

		processInput();
		drawGame();
		time_ += 0.001f;
		calculateFPS();

		// print only every 10 frames
		static int frame_count = 0;
		frame_count++;
		if (frame_count == 10)
		{
			std::cout << fps_ << std::endl;
			frame_count = 0;
		}

		// limit fps to max fps
		float frame_ticks = SDL_GetTicks() - start_ticks;
		if (1000.0f / max_fps_ > frame_ticks)
		{
			SDL_Delay(1000.0f / max_fps_ - frame_ticks);
		}
	}
}

void MainGame::processInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			game_state_ = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			// std::cout << event.motion.x << " " << event.motion.y << std::endl;
			break;
		}
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
	GLuint timeLocation = color_program_.getUniformLocation("time");
	glUniform1f(timeLocation, time_);

	for (size_t i = 0; i < sprites_.size(); i++)
	{
		sprites_[i]->draw();
	}
	// sprite_.draw();
	glBindTexture(GL_TEXTURE_2D, 0);
	color_program_.unuse();

	window_.swapBuffer();
}

void MainGame::calculateFPS()
{
	static const int NUM_SAMPLES = 10;
	static float frame_times[NUM_SAMPLES];
	static int curr_frame = 0;

	static float prev_ticks = SDL_GetTicks();
	float curr_ticks;
	curr_ticks = SDL_GetTicks();

	frame_time_ = curr_ticks - prev_ticks;
	frame_times[curr_frame % NUM_SAMPLES] = frame_time_;

	prev_ticks = curr_ticks;

	curr_frame++;
	int count;
	if (curr_frame < NUM_SAMPLES)
	{
		count = curr_frame;
	}
	else
	{
		count = NUM_SAMPLES;
	}

	float frame_time_avg = 0;
	for (size_t i = 0; i < count; i++)
	{
		frame_time_avg += frame_times[i];
	}
	frame_time_avg /= static_cast<float> (count);

	if (frame_time_avg > 0)
	{
		fps_ = 1000.0f / frame_time_avg;
	}
	else
	{
		fps_ = 60.0f;
	}
}
