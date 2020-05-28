#include "Stronghold.hpp"

base::Stronghold::Stronghold()
	:_hp(100), _activeCollider(true)
{
}

void base::Stronghold::damage(int dmg)
{
	_hp -= dmg;
}
