#include "MainGame.h"

#include <iostream>

#include <SkeletonEngine/SkeletonEngine.h>
#include <SkeletonEngine/Timing.h>

MainGame::MainGame(): height_(768), width_(1024), game_state_(GameState::PLAY), fps_(0)
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
}


void MainGame::gameLoop()
{
	SkeletonEngine::FpsLimiter fps_limiter;
	fps_limiter.setMaxFPS(60.0f);

	while (game_state_ == GameState::PLAY)
	{
		fps_limiter.begin();

		processInput();
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

	levels_[current_level_]->draw();

	texture_program_.unuse();

	// swap buffer to draw to game
	window_.swapBuffer();
}
