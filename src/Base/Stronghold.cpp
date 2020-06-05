#include "Stronghold.hpp"

base::Stronghold::Stronghold()
	:_hp(50), _activeCollider(true)
{
}

void base::Stronghold::damage(int dmg)
{
	_hp -= dmg;
}
