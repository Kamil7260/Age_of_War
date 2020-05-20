#include "Stronghold.hpp"

base::Stronghold::Stronghold()
	:_hp(100)
{
}

base::Stronghold::Stronghold(Stronghold&& source) noexcept
	:_hp(source._hp)
{
}

base::Stronghold& base::Stronghold::operator=(Stronghold&& source) noexcept
{
	_hp = source._hp;
	return *this;
}

void base::Stronghold::damage(int dmg)
{
	_hp -= dmg;
}
