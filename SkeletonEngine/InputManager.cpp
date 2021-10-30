#include "InputManager.h"

namespace SkeletonEngine
{
	InputManager::InputManager(): mouse_coords_(0.0f)
	{
	}

	InputManager::~InputManager()
	{
	}

	void InputManager::pressKey(unsigned int key_id)
	{
		key_map_[key_id] = true;
	}

	void InputManager::releaseKey(unsigned int key_id)
	{
		key_map_[key_id] = false;
	}

	bool InputManager::isKeyPressed(unsigned int key_id)
	{
		auto it = key_map_.find(key_id);
		if (it != key_map_.end())
		{
			return it->second;
		}

		return false;
	}

	void InputManager::setMouseCoords(float x, float y)
	{
		mouse_coords_.x = x;
		mouse_coords_.y = y;
	}
}
