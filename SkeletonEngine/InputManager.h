#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

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

		void setMouseCoords(float x, float y);
		glm::vec2 getMouseCoords() const { return mouse_coords_; }

	private:
		std::unordered_map<unsigned int, bool> key_map_;
		glm::vec2 mouse_coords_;
	};
}

