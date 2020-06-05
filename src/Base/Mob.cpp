#include "Mob.hpp"

base::Mob::Mob(int hp, int minattack, int maxattack, float speedattack, float speedmove, int income)
	:m_hp(hp),m_attack(minattack),m_maxAttack(maxattack), m_speedAttack(speedattack),m_speedMove(speedmove), m_activeCollider(true),m_income(income)
{
	m_tag = "Mob";
}

void base::Mob::damage(int dmg)
{
	m_hp -= dmg;
}

void base::Mob::setTeam(const team& tm)
{
	m_team = tm;
	if (m_team == base::team::enemy)
	{
		m_sprite->scale(sf::Vector2f(-1.f, 1.f));
		m_speedMove *= -1.f;
	}
}

void base::Mob::setSpeedMove(const float speed)
{
	m_speedMove = speed;
}

void base::Mob::setSpeedAttack(const float speed)
{
	m_speedAttack = speed;
}

void base::Mob::setAttack(const int min, const int max)
{
	m_attack = min;
	m_maxAttack = max;
}
