#include "Enemy.hpp"
#include "../Core/ResourceManager.hpp"
#include "../Core/Application.hpp"
#include "../GameClass/Melee.hpp"
#include "../GameClass/Range.hpp"

Enemy::Enemy()
	:_timer(0.f)
{
	_currentAge = "I";
	_team = base::team::enemy;
	_sprite.setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/base/1.png"));
	setScale(sf::Vector2f(-1.f, 1.f));
	_sprite.setOrigin(275.f, 170.f);
	_myColider = { 131.f,131.f,131.f,131.f };

	_mobTemplate.at(0) = base::loadUnitFromJson(0, _currentAge);
	_mobTemplate.at(1) = base::loadUnitFromJson(1, _currentAge);
	_mobTemplate.at(2) = base::loadUnitFromJson(2, _currentAge);
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
		std::random_device mch;
		std::default_random_engine generator(mch());
		std::uniform_int_distribution<int> distribution(1, 6);
		int attack_roll = distribution(generator);

		if (attack_roll == 6)
		{
			auto& k = _mobTemplate.at(2);
			std::unique_ptr<Melee> man = std::make_unique<Melee>(k.collider, k.hp, k.minDMG, k.maxDMG, k.speedAttack, k.speedMove, k.income);
			man->setScale(sf::Vector2f(k.scale.x, k.scale.y));
			man->setAnimatorName(k.name);
			man->setAnimationSpeed(k.animationSpeed);
			man->setPosition(sf::Vector2f(_position.x - 100, _position.y + 75));
			core::Renderer::getInstance().addEnemyObject(std::move(man));
		}
		else if (attack_roll < 3)
		{
			auto& k = _mobTemplate.at(1);
			std::unique_ptr<Range> man = std::make_unique<Range>(k.collider, k.hp, k.minDMG, k.maxDMG, k.range, k.speedAttack, k.speedMove, k.income);
			man->setScale(sf::Vector2f(k.scale.x, k.scale.y));
			man->setAnimatorName(k.name);
			man->setAnimationSpeed(k.animationSpeed);
			man->setPosition(sf::Vector2f(_position.x - 100, _position.y + 75));
			core::Renderer::getInstance().addEnemyObject(std::move(man));
		}
		else {
			auto& k = _mobTemplate.at(0);
			std::unique_ptr<Melee> man = std::make_unique<Melee>(k.collider, k.hp, k.minDMG, k.maxDMG, k.speedAttack, k.speedMove, k.income);
			man->setScale(sf::Vector2f(k.scale.x, k.scale.y));
			man->setAnimatorName(k.name);
			man->setAnimationSpeed(k.animationSpeed);
			man->setPosition(sf::Vector2f(_position.x - 100, _position.y + 75));
			core::Renderer::getInstance().addEnemyObject(std::move(man));
		}
		_timer = 0;
	}

}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
}
