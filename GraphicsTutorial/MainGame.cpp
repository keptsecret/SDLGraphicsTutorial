#include "MainGame.h"

#include <iostream>

#include "Errors.h"
#include "ImageLoader.h"

MainGame::MainGame(): window_(nullptr), screen_width_(1024), screen_height_(768), game_state_(GameState::PLAY), time_(0)
{
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();

	sprite_.init(-1.0, -1.0, 2.0, 2.0);

	player_texture_ = ImageLoader::loadPNG("./Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

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
		processInput();
		drawGame();
		time_ += 0.001f;
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

	color_program_.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, player_texture_.id);
	GLint texture_location = color_program_.getUniformLocation("samplerTex");
	glUniform1i(texture_location, 0);

	// commented out to avoid code optimization when time is not used in shader code
	GLuint timeLocation = color_program_.getUniformLocation("time");
	glUniform1f(timeLocation, time_);

	sprite_.draw();
	glBindTexture(GL_TEXTURE_2D, 0);
	color_program_.unuse();

	SDL_GL_SwapWindow(window_);
}


