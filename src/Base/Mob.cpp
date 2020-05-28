#include "Mob.hpp"

base::Mob::Mob(int hp, int minattack, int maxattack, float speedattack, float speedmove)
	:_hp(hp),_attack(minattack),_maxAttack(maxattack), _speedAttack(speedattack),_speedMove(speedmove), _activeCollider(true)
{
	_tag = "Mob";
}

void base::Mob::damage(int dmg)
{
	_hp -= dmg;
}

void base::Mob::setTeam(const team& tm)
{
	_team = tm;
	if (_team == base::team::enemy)
	{
		_sprite->scale(sf::Vector2f(-1.f, 1.f));
		_speedMove *= -1.f;
	}
}

void base::Mob::setSpeedMove(const float speed)
{
	_speedMove = speed;
}

void base::Mob::setSpeedAttack(const float speed)
{
	_speedAttack = speed;
}

void base::Mob::setAttack(const int min, const int max)
{
	_attack = min;
	_maxAttack = max;
}
