#include <random>

#include "Bullet.hpp"
#include "../Base/Mob.hpp"
#include "../Core/Renderer.hpp"
#include "../Core/Application.hpp"
Bullet::Bullet(int minDMG, int maxDMG,float speed, const sf::Vector2f& dir)
	:m_direction(dir), m_speed(speed), m_minDMG(minDMG), m_maxDMG(maxDMG)
{
	m_myColider = { 0,30,0,30 };
	m_tag = "Bullet";
}

void Bullet::setTexture(const sf::Texture& tex)
{
	m_sprite.setTexture(tex);
}

void Bullet::setPosition(const sf::Vector2f& pos)
{
	m_sprite.setPosition(pos);
	m_position = pos;
}

void Bullet::setScale(const sf::Vector2f& sca)
{
	m_sprite.setScale(sca);
	m_scale = sca;
}

void Bullet::move(const sf::Vector2f& delta)
{
	m_sprite.move(delta);
	m_position += delta;
}

void Bullet::onCollision(std::unique_ptr<Actor>& collision)
{
	if (collision->getTag() == "Mob")
	{
		auto ptr = static_cast<base::Mob*>(collision.get());
		
		std::random_device mch;
		std::default_random_engine generator(mch());
		std::uniform_int_distribution<int> distribution(m_minDMG, m_maxDMG);
		int attack_roll = distribution(generator);

		ptr->damage(attack_roll);
		remove();
		core::Renderer::getInstance().clearNoActive();
	}
}

void Bullet::onUpdate()
{
	auto k = core::Application::getInstance().getTime();
	k *= m_speed;
	move(sf::Vector2f(m_direction.x * k, m_direction.y * k));

	if (m_position.y > 1000)
	{
		remove();
		core::Renderer::getInstance().clearNoActive();
	}
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}
