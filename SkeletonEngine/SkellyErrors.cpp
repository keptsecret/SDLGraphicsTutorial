#include "SkellyErrors.h"

#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>

namespace SkeletonEngine
{

	void fatalError(const std::string& error_string)
	{
		std::cout << error_string << std::endl;
		std::cout << "Enter any key to quit...";
		int tmp;
		std::cin >> tmp;
		SDL_Quit();
		exit(1);
	}

}
