#include "Bullet.h"

#include "SkeletonEngine/ResourceManager.h"

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifetime)
{
	position_ = pos;
	direction_ = dir;
	speed_ = speed;
	lifetime_ = lifetime;
}

Bullet::~Bullet()
{
}

void Bullet::draw(SkeletonEngine::SpriteBatch& sprite_batch)
{
	SkeletonEngine::Color color{ 255, 255, 255, 255 };
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	// TODO: bad practice to use static texture but just as placeholder for now
	static SkeletonEngine::GLTexture texture = SkeletonEngine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	glm::vec4 pos_size = glm::vec4(position_.x, position_.y, 30.0f, 30.0f);
	sprite_batch.draw(pos_size, uv, texture.id, 0.0f, color);
}

bool Bullet::update()
{
	position_ += direction_ * speed_;
	lifetime_--;
	if (lifetime_ == 0)
	{
		return true;
	}
	return false;
}
