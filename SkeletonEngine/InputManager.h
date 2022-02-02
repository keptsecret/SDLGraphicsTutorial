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

		void update();

		void pressKey(unsigned int key_id);
		void releaseKey(unsigned int key_id);
		// returns true if key is held down
		bool isKeyDown(unsigned int key_id);
		// returns true if key was just pressed
		bool isKeyPressed(unsigned int key_id);

		void setMouseCoords(float x, float y);
		glm::vec2 getMouseCoords() const { return mouse_coords_; }

	private:
		std::unordered_map<unsigned int, bool> key_map_;
		std::unordered_map<unsigned int, bool> prev_key_map_;
		glm::vec2 mouse_coords_;

		bool wasKeyDown(unsigned int key_id);
	};
}

