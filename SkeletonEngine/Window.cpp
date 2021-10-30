#include "Window.h"

#include "Errors.h"

namespace SkeletonEngine
{

	Window::Window()
	{
	}

	Window::~Window()
	{
	}

	int Window::create(std::string window_name, int screen_width, int screen_height, unsigned curr_flag)
	{
		Uint32 flags = SDL_WINDOW_OPENGL;

		if (curr_flag & INVISIBLE)
		{
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (curr_flag & FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (curr_flag & BORDERLESS)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		sdl_window_ = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_OPENGL);

		if (sdl_window_ == nullptr)
		{
			fatalError("SDL Window could not be created!");
		}

		SDL_GLContext gl_context = SDL_GL_CreateContext(sdl_window_);
		if (gl_context == nullptr)
		{
			fatalError("SDL Context could not be created!");
		}

		GLenum error = glewInit();
		if (error != GLEW_OK)
		{
			fatalError("Could not initialize glew!");
		}

		// check opengl version
		std::printf("***    OpenGL version: %s    ***\n", glGetString(GL_VERSION));

		glClearColor(0.f, 0.f, 1.0f, 1.0f);

		SDL_GL_SetSwapInterval(0);		// set vsync on (1), off (0)

		// enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::swapBuffer()
	{
		SDL_GL_SwapWindow(sdl_window_);
	}

}
