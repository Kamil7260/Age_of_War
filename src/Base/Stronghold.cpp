#include "Stronghold.hpp"

base::Stronghold::Stronghold()
	:m_hp(500), m_activeCollider(true)
{
}

void base::Stronghold::damage(int dmg)
{
	m_hp -= dmg;
}
