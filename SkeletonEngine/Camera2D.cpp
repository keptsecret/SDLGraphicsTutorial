#include "Camera2D.h"

namespace SkeletonEngine
{

	Camera2D::Camera2D() : need_matrix_update_(true),
		scale_(1.0f),
		position_(0.0f, 0.0f),
		camera_matrix_(1.0f),
		ortho_matrix_(1.0f),
		screen_width_(500),
		screen_height_(500)
	{
	}

	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screen_width, int screen_height)
	{
		screen_width_ = screen_width;
		screen_height_ = screen_height;
		ortho_matrix_ = glm::ortho(0.0f, static_cast<float>(screen_width), 0.0f, static_cast<float>(screen_height));
	}

	void Camera2D::update()
	{
		if (need_matrix_update_)
		{
			// Camera Translation
			glm::vec3 translate(-position_.x + static_cast<float>(screen_width_) / 2, -position_.y + static_cast<float>(screen_height_) / 2, 0.0f);
			camera_matrix_ = glm::translate(ortho_matrix_, translate);

			// Camera Scale
			glm::vec3 scale(scale_, scale_, 0.0f);
			camera_matrix_ = glm::scale(glm::mat4(1.0f), scale) * camera_matrix_;

			need_matrix_update_ = false;
		}
	}

	glm::vec2 Camera2D::convertScreenToWorldCoords(glm::vec2 screen_coords)
	{
		//invert y direction
		screen_coords.y = screen_height_ - screen_coords.y;
		// center the coords to camera
		screen_coords -= glm::vec2(screen_width_ / 2, screen_height_ / 2);
		// scale the coords
		screen_coords /= scale_;
		// translate with camera position
		screen_coords += position_;
		return screen_coords;
	}
}
