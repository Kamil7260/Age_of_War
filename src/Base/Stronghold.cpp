#include "Stronghold.hpp"

base::Stronghold::Stronghold()
	:_hp(100), _activeCollider(true)
{
}

base::Stronghold::Stronghold(Stronghold&& source) noexcept
	:_hp(source._hp), _activeCollider(source._activeCollider)
{
}

base::Stronghold& base::Stronghold::operator=(Stronghold&& source) noexcept
{
	_hp = source._hp;
	_activeCollider = source._activeCollider;
	return *this;
}

void base::Stronghold::damage(int dmg)
{
	_hp -= dmg;
}
