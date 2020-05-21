#include "Mob.hpp"

base::Mob::Mob(int hp, int minattack, int maxattack, float speedattack, float speedmove)
	:_hp(hp),_attack(minattack),_maxAttack(maxattack), _speedAttack(speedattack),_speedMove(speedmove), _activeCollider(true)
{
	_tag = "Mob";
}

base::Mob::Mob(const Mob& source)
	:_hp(source._hp), _attack(source._attack), _maxAttack(source._maxAttack),
	_speedAttack(source._speedAttack), _speedMove(source._speedMove),
	_activeCollider(source._activeCollider)
{
	_tag = "Mob";
}

base::Mob::Mob(Mob&& source) noexcept
	:_hp(source._hp), _attack(source._attack), _maxAttack(source._maxAttack),
	_speedAttack(source._speedAttack), _speedMove(source._speedMove),
	_activeCollider(source._activeCollider)
{
	_tag = "Mob";
	source._hp = 0;
	source._attack = 0;
	source._speedAttack = 0;
	source._speedMove = 0;
	source._activeCollider = false;
}

base::Mob& base::Mob::operator=(const Mob& source)
{
	_tag = "Mob";
	_maxAttack = source._maxAttack;
	_hp = source._hp;
	_attack = source._attack;
	_speedAttack = source._speedAttack;
	_speedMove = source._speedMove;
	_activeCollider = source._activeCollider;
	return *this;
}

base::Mob& base::Mob::operator=(Mob&& source) noexcept
{
	_tag = "Mob";
	_hp = source._hp;
	_maxAttack = source._maxAttack;
	_attack = source._attack;
	_speedAttack = source._speedAttack;
	_speedMove = source._speedMove;
	_activeCollider = source._activeCollider;
	source._activeCollider = false;
	source._hp = 0;
	source._attack = 0;
	source._speedAttack = 0;
	source._speedMove = 0;
	return *this;
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
