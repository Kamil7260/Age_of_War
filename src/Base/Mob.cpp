#include "Mob.hpp"

base::Mob::Mob(int hp, int attack, float speedattack, float speedmove)
	:_hp(hp),_attack(attack), _speedAttack(speedattack),_speedMove(speedmove)
{
	_tag = "Mob";
}

base::Mob::Mob(const Mob& source)
	:_hp(source._hp), _attack(source._attack),
	_speedAttack(source._speedAttack), _speedMove(source._speedMove)
{
	_tag = "Mob";
}

base::Mob::Mob(Mob&& source) noexcept
	:_hp(source._hp), _attack(source._attack),
	_speedAttack(source._speedAttack), _speedMove(source._speedMove)
{
	_tag = "Mob";
	source._hp = 0;
	source._attack = 0;
	source._speedAttack = 0;
	source._speedMove = 0;
}

base::Mob& base::Mob::operator=(const Mob& source)
{
	_tag = "Mob";
	_hp = source._hp;
	_attack = source._attack;
	_speedAttack = source._speedAttack;
	_speedMove = source._speedMove;
	return *this;
}

base::Mob& base::Mob::operator=(Mob&& source) noexcept
{
	_tag = "Mob";
	_hp = source._hp;
	_attack = source._attack;
	_speedAttack = source._speedAttack;
	_speedMove = source._speedMove;
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
