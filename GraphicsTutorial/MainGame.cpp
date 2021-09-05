#include "MainGame.h"

#include <iostream>

void fatalError(std::string errorString)
{
	std::cout << errorString << std::endl;
	std::cout << "Enter any key to quit...";
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
}

MainGame::MainGame()
{
	window_ = nullptr;
	screen_width_ = 1024;
	screen_height_ = 768;
	game_state_ = GameState::PLAY;
}

MainGame::~MainGame()
{
	
}

void MainGame::run()
{
	initSystems();

	sprite_.init(-1, -1, 1, 1);

	gameLoop();
}

void MainGame::initSystems()
{
	// Initialize SDL with everything
	SDL_Init(SDL_INIT_EVERYTHING);

	window_ = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width_, screen_height_, SDL_WINDOW_OPENGL);

	if (window_ == nullptr)
	{
		fatalError("SDL Window could not be created!");
	}

	SDL_GLContext gl_context = SDL_GL_CreateContext(window_);
	if (gl_context == nullptr)
	{
		fatalError("SDL Context could not be created!");
	}

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		fatalError("Could not initialize glew!");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glClearColor(0.f, 0.f, 1.0f, 1.0f);
}

void MainGame::gameLoop()
{
	while (game_state_ != GameState::EXIT)
	{
		processInput();
		drawGame();
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
			std::cout << event.motion.x << " " << event.motion.y << std::endl;
			break;
		}
	}
}

void MainGame::drawGame()
{
	glClearDepth(1.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sprite_.draw();

	SDL_GL_SwapWindow(window_);
}


