#pragma once

#include <SDL.h>
#include <vector>
#include <GL/glew.h>

#include "Sprite.h"
#include "GLSLProgram.h"
#include "GLTexture.h"

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
	GLSLProgram color_program_;

	std::vector<Sprite*> sprites_;

	float time_;
	float fps_;
	float frame_time_;
	float max_fps_;

	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();
	void calculateFPS();
};
