#include <SDL/SDL.h>
#include <glew/glew.h>

#include "SkeletonEngine.h"

namespace SkeletonEngine
{

	int init()
	{
		// Initialize SDL with everything
		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);			// tell SDL we want a double buffered window to prevent flickering

		return 0;
	}

}
