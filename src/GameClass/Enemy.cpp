#include "Enemy.hpp"
#include "../Core/ResourceManager.hpp"
#include "../Core/Application.hpp"
#include "../GameClass/Melee.hpp"
#include "../GameClass/Range.hpp"
#include "HpBar.hpp"
Enemy::Enemy()
	:m_timer(0.f)
{
	m_currentAge = "I";
	m_tag = "Enemy";
	m_team = base::team::enemy;
	m_sprite.setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/base/I.png"));
	setScale(sf::Vector2f(-1.f, 1.f));
	m_sprite.setOrigin(275.f, 170.f);
	m_myColider = { 131.f,131.f,131.f,131.f };

	auto hp = std::make_unique<HpBar>(&m_hp, 24, 250);
	hp->setPosition(sf::Vector2f(70.f, 800));
	hp->setCallbackOnDie([&]()->void {
		auto tex = core::ResourceManager<sf::Texture>::getInstance().get("Assets/frames/1.png");
		core::Application::getInstance().freezeScreen(*tex);
		});
	hp->setFont(*core::ResourceManager<sf::Font>::getInstance().get("Assets/fonts/3.ttf"));
	hp->setCharacterSize(15);
	core::Renderer::getInstance().addObject(std::move(hp), base::object_type::gui);

	m_mobTemplate.at(0) = base::loadUnitFromJson(0, m_currentAge);
	m_mobTemplate.at(1) = base::loadUnitFromJson(1, m_currentAge);
	m_mobTemplate.at(2) = base::loadUnitFromJson(2, m_currentAge);
}

void Enemy::setPosition(const sf::Vector2f& pos)
{
	m_position = pos;
	m_sprite.setPosition(pos);
}

void Enemy::setScale(const sf::Vector2f& sca)
{
	m_scale = sca;
	m_sprite.setScale(sca);
}

void Enemy::move(const sf::Vector2f& delta)
{
	m_sprite.move(delta);
	m_position += delta;
}

void Enemy::onUpdate()
{
	m_timer += core::Application::getInstance().getTime();

	if (m_timer >= 5)
	{
		std::random_device mch;
		std::default_random_engine generator(mch());
		std::uniform_int_distribution<int> distribution(1, 6);
		int attack_roll = distribution(generator);

		if (attack_roll == 6)
		{
			auto& k = m_mobTemplate.at(2);
			std::unique_ptr<Melee> man = std::make_unique<Melee>(k.collider, k.hp, k.minDMG, k.maxDMG, k.speedAttack, k.speedMove, k.income);
			man->setScale(sf::Vector2f(k.scale.x, k.scale.y));
			man->setAnimatorName(k.name);
			man->setAnimationSpeed(k.animationSpeed);
			man->setPosition(sf::Vector2f(m_position.x - 100, m_position.y + 75));
			core::Renderer::getInstance().addEnemyObject(std::move(man));
		}
		else if (attack_roll < 3)
		{
			auto& k = m_mobTemplate.at(1);
			std::unique_ptr<Range> man = std::make_unique<Range>(k.collider, k.hp, k.minDMG, k.maxDMG, k.range, k.speedAttack, k.speedMove, k.income);
			man->setScale(sf::Vector2f(k.scale.x, k.scale.y));
			man->setAnimatorName(k.name);
			man->setAnimationSpeed(k.animationSpeed);
			man->setPosition(sf::Vector2f(m_position.x - 100, m_position.y + 75));
			core::Renderer::getInstance().addEnemyObject(std::move(man));
		}
		else {
			auto& k = m_mobTemplate.at(0);
			std::unique_ptr<Melee> man = std::make_unique<Melee>(k.collider, k.hp, k.minDMG, k.maxDMG, k.speedAttack, k.speedMove, k.income);
			man->setScale(sf::Vector2f(k.scale.x, k.scale.y));
			man->setAnimatorName(k.name);
			man->setAnimationSpeed(k.animationSpeed);
			man->setPosition(sf::Vector2f(m_position.x - 100, m_position.y + 75));
			core::Renderer::getInstance().addEnemyObject(std::move(man));
		}
		m_timer = 0;
	}

}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}
