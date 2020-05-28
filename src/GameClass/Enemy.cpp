#include "Enemy.hpp"
#include "../Core/ResourceManager.hpp"
#include "../Core/Application.hpp"
#include "../GameClass/Melee.hpp"

Enemy::Enemy()
	:_timer(0.f)
{
	_team = base::team::enemy;
	_sprite.setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/base/1.png"));
	setScale(sf::Vector2f(-1.f, 1.f));
	_sprite.setOrigin(275.f, 170.f);
	_myColider = { 131.f,131.f,131.f,131.f };
}

void Enemy::setPosition(const sf::Vector2f& pos)
{
	_position = pos;
	_sprite.setPosition(pos);
}

void Enemy::setScale(const sf::Vector2f& sca)
{
	_scale = sca;
	_sprite.setScale(sca);
}

void Enemy::move(const sf::Vector2f& delta)
{
	_sprite.move(delta);
	_position += delta;
}

void Enemy::onUpdate()
{
	_timer += core::Application::getInstance().getTime();

	if (_timer >= 5)
	{
		std::unique_ptr<Melee> man = std::make_unique<Melee>();
		man->setAnimatorName("caveman");
		man->setPosition(sf::Vector2f(_position.x - 100, 850));
		core::Renderer::getInstance().addEnemyObject(std::move(man));
		_timer = -1000;
	}

}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
}
