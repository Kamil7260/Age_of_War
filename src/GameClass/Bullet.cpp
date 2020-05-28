#include <random>

#include "Bullet.hpp"
#include "../Base/Mob.hpp"
#include "../Core/Renderer.hpp"
#include "../Core/Application.hpp"
Bullet::Bullet(int minDMG, int maxDMG,float speed, const sf::Vector2f& dir)
	:_direction(dir), _speed(speed), _minDMG(minDMG), _maxDMG(maxDMG)
{
	_myColider = { 0,30,0,30 };
}

void Bullet::setTexture(const sf::Texture& tex)
{
	_sprite.setTexture(tex);
}

void Bullet::setPosition(const sf::Vector2f& pos)
{
	_sprite.setPosition(pos);
	_position = pos;
}

void Bullet::setScale(const sf::Vector2f& sca)
{
	_sprite.setScale(sca);
	_scale = sca;
}

void Bullet::move(const sf::Vector2f& delta)
{
	_sprite.move(delta);
	_position += delta;
}

void Bullet::onCollision(std::unique_ptr<Actor>& collision)
{
	if (collision->getTag() == "Mob")
	{
		auto ptr = static_cast<base::Mob*>(collision.get());
		
		std::random_device mch;
		std::default_random_engine generator(mch());
		std::uniform_int_distribution<int> distribution(_minDMG, _maxDMG);
		int attack_roll = distribution(generator);

		ptr->damage(attack_roll);
		remove();
		core::Renderer::getInstance().clearNoActive();
	}
}

void Bullet::onUpdate()
{
	auto k = core::Application::getInstance().getTime();
	k *= _speed;
	move(sf::Vector2f(_direction.x * k, _direction.y * k));

	if (_position.y > 1000)
	{
		remove();
		core::Renderer::getInstance().clearNoActive();
	}
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
}
