#pragma once
#include <SDL/SDL.h>
#include <string>
#include <glew/glew.h>

namespace SkeletonEngine
{


	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string window_name, int screen_width, int screen_height, unsigned int curr_flag);
		void swapBuffer();

		int getScreenWidth() { screen_width_; }
		int getScreenHeight() { screen_height_; }

	private:
		SDL_Window* sdl_window_;
		int screen_width_, screen_height_;
	};

}
