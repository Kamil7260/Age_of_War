#include <random>

#include "Melee.hpp"
#include "../Core/Application.hpp"
#include "../Core/ResourceManager.hpp"
#include "TextDisplay.hpp"
#include "../Base/Stronghold.hpp"

Melee::Melee(const base::collider& collider, int hp, int attack, int maxAttack, float speedAttack, float speedMove, int income)
	:Mob(hp, attack,maxAttack, speedAttack, speedMove, income),m_timer(0.f),m_attackTimer(0.f), m_touchEnemy(false), m_isCollided(false), m_enableAttack(true), m_died(false)
{
	m_myColider = collider;
}

void Melee::setAnimatorName(const std::string& name)
{
	m_walkClip = name;
	m_walkClip += "_walk";
	addClip(core::Application::getInstance().getClip(m_walkClip), m_walkClip);

	m_idleClip = name;
	m_idleClip += "_idle";
	addClip(core::Application::getInstance().getClip(m_idleClip), m_idleClip);

	m_dieClip = name;
	m_dieClip += "_die";
	addClip(core::Application::getInstance().getClip(m_dieClip), m_dieClip);
	auto d = m_container.find(m_dieClip);
	d->second.setCallback([&]()->void {
		m_died = true;
		m_timer = 0.f;
		});

	m_attackClip = name;
	m_attackClip += "_attack";
	addClip(core::Application::getInstance().getClip(m_attackClip), m_attackClip);
	
	auto k = m_container.find(m_attackClip);
	k->second.setCallback([&]()->void {
		m_enableAttack = true;
		});
}

void Melee::onUpdate()
{
	float delta = core::Application::getInstance().getTime();
	m_timer += delta;
	if (m_hp > 0)
	{
		if (m_currentClipName == m_attackClip && m_enableAttack)
		{
			m_attackTimer += delta;
		}
		else m_attackTimer = 0;

		if (!m_isCollided)
		{
			if (!m_isRunning || m_currentClipName != m_walkClip)
			{
				play(m_walkClip);
			}
			move(sf::Vector2f(m_speedMove * delta, 0.f));
		}
		else if (!m_isRunning) {
			if (!m_touchEnemy)
				play(m_idleClip);
			else {
				play(m_attackClip);
			}
		}
	}
	else{
		if (m_died && m_timer > 2.f)
		{
			m_isActive = false;
			core::Renderer::getInstance().clearNoActive();
		}
	}
	m_touchEnemy = false;
	m_isCollided = false;
	updateAnimator();
}

void Melee::damage(int dmg)
{
	Mob::damage(dmg);
	if (m_hp <= 0)
	{
		if (m_team != base::team::player)
		{
			auto ptr = std::make_unique<TextDisplay>(2.5f, 70.f);
			ptr->setPosition(m_position);
			ptr->setCharacterSize(20);
			ptr->setFont(*core::ResourceManager<sf::Font>::getInstance().get("Assets/fonts/3.ttf"));
			ptr->setIncome(m_income);
			ptr->setColor(sf::Color::Yellow);
			ptr->setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/other/5.png"));
			core::Renderer::getInstance().addObject(std::move(ptr), base::object_type::gui);
		}

		m_position = { 0.f,0.f };
		m_activeCollider = false;
		if(m_currentClipName != m_dieClip)
		play(m_dieClip);
	}
}

void Melee::setPosition(const sf::Vector2f& pos)
{
	m_position = pos;
	m_sprite->setPosition(pos);
}

void Melee::onCollision(std::unique_ptr<base::Actor>& collision)
{
	if (collision->getTag() == "Bullet")
		return;

	if (collision->getTeam() != m_team)
	{
		m_touchEnemy = true;
		m_isCollided = true;
		if (m_attackTimer>m_speedAttack && collision->getTag() == "Mob")
		{
			m_attackTimer = 0.f;
			m_enableAttack = false;
			if (m_hp <= 0) return;
			std::random_device mch;
			std::default_random_engine generator(mch());
			std::uniform_int_distribution<int> distribution(m_attack,m_maxAttack);
			int attack_roll = distribution(generator);

			auto ptr = static_cast<base::Mob*>(collision.get());
			ptr->damage(attack_roll);
			return;
		}

		if (collision->getTag() == "Player" || collision->getTag() == "Enemy")
		{
			if (m_attackTimer > m_speedAttack)
			{
				m_attackTimer = 0.f;
				m_enableAttack = false;
				if (m_hp <= 0) return;
				std::random_device mch;
				std::default_random_engine generator(mch());
				std::uniform_int_distribution<int> distribution(m_attack, m_maxAttack);
				int attack_roll = distribution(generator);

				auto ptr = static_cast<base::Stronghold*>(collision.get());
				ptr->damage(attack_roll);
			}
		}

		if (m_currentClipName != m_attackClip && m_hp>0)
		{
			play(m_attackClip);
			return;
		}
	}

	if (m_hp <= 0)
		return;

	if (m_speedMove > 0)
	{
		if (collision->getPosition().x >= m_position.x)
		{
			if(m_currentClipName != m_attackClip)
			play(m_idleClip);
			m_isCollided = true;
		}
		return;
	}
	else{
		if (collision->getPosition().x <= m_position.x)
		{
			if (m_currentClipName != m_attackClip)
			play(m_idleClip);
			m_isCollided = true;
		}
		return;
	}
}


