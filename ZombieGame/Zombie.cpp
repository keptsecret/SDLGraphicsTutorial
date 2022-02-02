#include "Zombie.h"

#include "Human.h"

Zombie::Zombie()
{
}

Zombie::~Zombie()
{
}

void Zombie::init(float speed, glm::vec2 pos)
{
    health_ = 150.0f;
    speed_ = speed;
    position_ = pos;
    color_ = SkeletonEngine::ColorRGBA8{ 0, 160, 0, 255 };
}

void Zombie::update(const std::vector<std::string>& level_data,
                    std::vector<Human*>& humans,
                    std::vector<Zombie*>& zombies)
{
    Human* target = getNearestHuman(humans);

    if (target != nullptr)
    {
        glm::vec2 direction = glm::normalize(target->getPosition() - position_);
        position_ += direction * speed_;
    }

    collideWithLevel(level_data);
}

Human* Zombie::getNearestHuman(std::vector<Human*> humans)
{
    Human* closest_human = nullptr;
    float smallest_dist = 999999.0f;

    for (Human* h : humans)
    {
        glm::vec2 dist_vec = h->getPosition() - position_;
        float distance = glm::length(dist_vec);

        if (distance < smallest_dist)
        {
            smallest_dist = distance;
            closest_human = h;
        }
    }

    return closest_human;
}
