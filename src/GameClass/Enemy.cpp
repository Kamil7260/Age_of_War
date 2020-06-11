#include "Enemy.hpp"
#include "../Core/ResourceManager.hpp"
#include "../Core/Application.hpp"
#include "../GameClass/Melee.hpp"
#include "../GameClass/Range.hpp"
#include "HpBar.hpp"
Enemy::Enemy()
	:m_timer(0.f), m_currentAge(0), m_groupOverAll(14), m_breakTime(0),m_exp(0), m_requiredExp(1500)
{
	m_tag = "Enemy";
	m_team = base::team::enemy;
	setScale(sf::Vector2f(-1.f, 1.f));
	m_sprite.setOrigin(275.f, 170.f);
	m_myColider = { 131.f,131.f,131.f,131.f };
	m_ages = { "I","II","III","IV","V" };
	
	auto hp = std::make_unique<HpBar>(&m_hp, 24, 250);
	hp->setPosition(sf::Vector2f(m_position.x + 100, 800));
	hp->setTag("EnemyHp");
	hp->dynamicDraw(true);
	hp->setCallbackOnDie([&]()->void {
		auto tex = core::ResourceManager<sf::Texture>::getInstance().get("Assets/frames/1.png");
		core::Application::getInstance().freezeScreen(*tex);
		});
	hp->setFont(*core::ResourceManager<sf::Font>::getInstance().get("Assets/fonts/3.ttf"));
	hp->setCharacterSize(15);
	core::Renderer::getInstance().forceAddObject(std::move(hp), base::object_type::gui);
	loadNewAge();
}

void Enemy::setPosition(const sf::Vector2f& pos)
{
	m_position = pos;
	m_sprite.setPosition(pos);
	auto& hp = core::Renderer::getInstance().find("EnemyHp");
	hp->setPosition(sf::Vector2f(pos.x + 130.f, pos.y - 50.f));

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

	if (m_exp > m_requiredExp)
	{
		loadNewAge();
		m_requiredExp *= 2;
	}

	float delta = core::Application::getInstance().getTime();
	if (m_queue.empty())
		m_timer += delta;
	else {
		auto& ref = m_queue.back();
		ref.second -= delta;
		if (ref.second < 0.f)
		{
			ref.first->setPosition(sf::Vector2f(m_position.x - 100, m_position.y + 75));
			core::Renderer::getInstance().addEnemyObject(std::move(ref.first));
			m_queue.erase(m_queue.end() - 1);
		}
	}
	if (m_timer > m_breakTime)
	{
		std::random_device mch;
		std::default_random_engine generator(mch());
		std::uniform_int_distribution<int> rollType(1, 7);
		std::uniform_int_distribution<int> rollTime(13, 23);

		m_timer = 0;
		m_breakTime = rollTime(generator);
		
		int currentOverAll = 0;
		int attack_roll;
		while (currentOverAll < m_groupOverAll)
		{
			attack_roll = rollType(generator);
			currentOverAll += attack_roll;
			if (attack_roll == 6)
			{
				auto& k = m_mobTemplate.at(2);
				std::unique_ptr<Melee> man = std::make_unique<Melee>(k.collider, k.hp, k.minDMG, k.maxDMG, k.speedAttack, k.speedMove, k.income);
				man->setScale(sf::Vector2f(k.scale.x, k.scale.y));
				man->setAnimatorName(k.name);
				man->setAnimationSpeed(k.animationSpeed);
				man->setPosition(sf::Vector2f(m_position.x - 100, m_position.y + 75));
				m_queue.push_back(std::make_pair(std::move(man), k.spawnTime));
				//core::Renderer::getInstance().addEnemyObject(std::move(man));
			}
			else if (attack_roll < 3)
			{
				auto& k = m_mobTemplate.at(1);
				std::unique_ptr<Range> man = std::make_unique<Range>(k.collider, k.hp, k.minDMG, k.maxDMG, k.range, k.speedAttack, k.speedMove, k.income);
				man->setScale(sf::Vector2f(k.scale.x, k.scale.y));
				man->setAnimatorName(k.name);
				man->setAnimationSpeed(k.animationSpeed);
				man->setPosition(sf::Vector2f(m_position.x - 100, m_position.y + 75));
				//	core::Renderer::getInstance().addEnemyObject(std::move(man));
				m_queue.push_back(std::make_pair(std::move(man), k.spawnTime));
			}
			else {
				auto& k = m_mobTemplate.at(0);
				std::unique_ptr<Melee> man = std::make_unique<Melee>(k.collider, k.hp, k.minDMG, k.maxDMG, k.speedAttack, k.speedMove, k.income);
				man->setScale(sf::Vector2f(k.scale.x, k.scale.y));
				man->setAnimatorName(k.name);
				man->setAnimationSpeed(k.animationSpeed);
				man->setPosition(sf::Vector2f(m_position.x - 100, m_position.y + 75));
				//core::Renderer::getInstance().addEnemyObject(std::move(man));
				m_queue.push_back(std::make_pair(std::move(man), k.spawnTime));
			}
		}
		m_timer = 0;
	}

}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

void Enemy::loadNewAge()
{
	if (m_currentAge >= m_ages.size())
		return;
	m_sprite.setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/base/" + m_ages.at(m_currentAge) + ".png"));
	m_mobTemplate.at(0) = base::loadUnitFromJson(0, m_ages.at(m_currentAge));
	m_mobTemplate.at(1) = base::loadUnitFromJson(1, m_ages.at(m_currentAge));
	m_mobTemplate.at(2) = base::loadUnitFromJson(2, m_ages.at(m_currentAge));
	++m_currentAge;
}
