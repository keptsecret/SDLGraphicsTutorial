#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

namespace SkeletonEngine
{

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void init(int screen_width, int screen_height);
		void update();

		void setPosition(const glm::vec2& new_position) { position_ = new_position; need_matrix_update_ = true; }
		void setScale(float new_scale) { scale_ = new_scale; need_matrix_update_ = true; }

		glm::vec2 getPosition() { return position_; }
		float getScale() { return scale_; }
		glm::mat4 getCameraMatrix() { return camera_matrix_; }

	private:
		bool need_matrix_update_;
		float scale_;
		glm::vec2 position_;
		glm::mat4 camera_matrix_;
		glm::mat4 ortho_matrix_;
		int screen_width_, screen_height_;
	};
}
