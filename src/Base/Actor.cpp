#include "Actor.hpp"

base::Actor::Actor(const base::collider& box)
	:m_isActive(true), m_type(object_type::actor), m_team(team::none), m_myColider(box),m_rotation(0), m_dynamic(true)
{
}

