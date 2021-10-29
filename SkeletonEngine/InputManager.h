#pragma once

#include <unordered_map>

namespace SkeletonEngine
{
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void pressKey(unsigned int key_id);
		void releaseKey(unsigned int key_id);
		bool isKeyPressed(unsigned int key_id);

	private:
		std::unordered_map<unsigned int, bool> key_map_;
	};
}

