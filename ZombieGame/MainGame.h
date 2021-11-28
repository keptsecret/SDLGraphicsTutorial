#pragma once

#include <SkeletonEngine/Window.h>
#include <SkeletonEngine/GLSLProgram.h>
#include <SkeletonEngine/Camera2D.h>
#include <SkeletonEngine/InputManager.h>

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystem();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();

	SkeletonEngine::Window window_;
	SkeletonEngine::GLSLProgram texture_program_;
	SkeletonEngine::InputManager input_manager_;
	SkeletonEngine::Camera2D camera_;
};

