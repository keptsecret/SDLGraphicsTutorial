#pragma once

#include <SDL.h>
#include <GL/glew.h>

#include "Sprite.h"

enum class GameState {PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	SDL_Window* window_;
	int screen_width_;
	int screen_height_;
	GameState game_state_;
	Sprite sprite_;

	void initSystems();
	void gameLoop();
	void processInput();
	void drawGame();
};
