#include "InputManager.h"

namespace SkeletonEngine
{
	InputManager::InputManager(): mouse_coords_(0.0f)
	{
	}

	InputManager::~InputManager()
	{
	}

	void InputManager::update()
	{
		// loop through key map, copy it to prev key map
		for (auto& it : key_map_)
		{
			prev_key_map_[it.first] = it.second;
		}
	}

	void InputManager::pressKey(unsigned int key_id)
	{
		key_map_[key_id] = true;
	}

	void InputManager::releaseKey(unsigned int key_id)
	{
		key_map_[key_id] = false;
	}

	bool InputManager::isKeyDown(unsigned int key_id)
	{
		auto it = key_map_.find(key_id);
		if (it != key_map_.end())
		{
			return it->second;
		}

		return false;
	}

	bool InputManager::isKeyPressed(unsigned key_id)
	{
		// check if was just pressed this frame
		if (isKeyDown(key_id) && !wasKeyDown(key_id))
		{
			return true;
		}
		return false;
	}

	void InputManager::setMouseCoords(float x, float y)
	{
		mouse_coords_.x = x;
		mouse_coords_.y = y;
	}

	bool InputManager::wasKeyDown(unsigned key_id)
	{
		auto it = prev_key_map_.find(key_id);
		if (it != prev_key_map_.end())
		{
			return it->second;
		}

		return false;
	}
}
