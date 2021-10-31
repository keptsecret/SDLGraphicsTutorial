#pragma once

#include <SDL.h>
#include <vector>
#include <GL/glew.h>

#include <SkeletonEngine/GLTexture.h>
#include <SkeletonEngine/GLSLProgram.h>
#include <SkeletonEngine/Sprite.h>
#include <SkeletonEngine/Window.h>
#include <SkeletonEngine/Camera2D.h>
#include <SkeletonEngine/SpriteBatch.h>
#include <SkeletonEngine/InputManager.h>
#include <SkeletonEngine/Timing.h>

#include "Bullet.h"

enum class GameState {PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	SkeletonEngine::Window window_;
	int screen_width_;
	int screen_height_;
	GameState game_state_;
	SkeletonEngine::Sprite sprite_;
	SkeletonEngine::GLSLProgram color_program_;

	SkeletonEngine::SpriteBatch sprite_batch_;

	SkeletonEngine::Camera2D camera_;
	SkeletonEngine::InputManager input_manager_;
	SkeletonEngine::FpsLimiter fps_limiter_;

	float time_;
	float fps_;
	float max_fps_;

	std::vector<Bullet> bullets_;

	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();
};
