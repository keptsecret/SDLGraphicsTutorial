#include "Errors.h"

#include <cstdlib>
#include <iostream>
#include <SDL.h>

void fatalError(const std::string& error_string)
{
	std::cout << error_string << std::endl;
	std::cout << "Enter any key to quit...";
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
	exit(1);
}
