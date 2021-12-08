#include "Human.h"

#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>

Human::Human(): frames_(0)
{
}

Human::~Human()
{
}

void Human::init(float speed, glm::vec2 pos)
{
    static std::mt19937 random_engine{static_cast<unsigned int>(time(nullptr))};
    static std::uniform_real_distribution<float> rand_dir{ -1.0f, 1.0f };

    color_ = SkeletonEngine::Color{ 200, 0, 200, 255 };

    speed_ = speed;
    position_ = pos;
    direction_ = glm::vec2(rand_dir(random_engine), rand_dir(random_engine));
    if (direction_.length() == 0) direction_ = glm::vec2{ 1.0f, 0.0f };

    direction_ = glm::normalize(direction_);
}

void Human::update(const std::vector<std::string>& level_data,
                   std::vector<Human*>& humans,
                   std::vector<Zombie*>& zombies)
{
    const float PI = 3.14159265359f;
    static std::mt19937 random_engine{ static_cast<unsigned int>(time(nullptr)) };
    static std::uniform_real_distribution<float> rand_rot{ -0.25f * PI, 0.25f * PI };   // between -45deg and 45 deg

    position_ += direction_ * speed_;
    // randomly change direction every 20 frames
    if (frames_ == 20)
    {
        direction_ = rotate(direction_, rand_rot(random_engine));
        frames_ = 0;
    }
    else
    {
        frames_++;
    }
    
	if(collideWithLevel(level_data))
	{
        direction_ = rotate(direction_, rand_rot(random_engine));
	}
}
