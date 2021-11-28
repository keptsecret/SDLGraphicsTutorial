#include "MainGame.h"

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
}

void MainGame::initSystem()
{
}

void MainGame::initShaders()
{
	texture_program_.compileShaders("Shaders/tex_shading.vert", "Shaders/tex_shading.frag");
	texture_program_.addAttribute("vertexPosition");
	texture_program_.addAttribute("vertexColor");
	texture_program_.addAttribute("vertexUV");
	texture_program_.linkShaders();
}

void MainGame::gameLoop()
{
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
}
