#include "Actor.hpp"

base::Actor::Actor(const base::collider& box)
	:_isActive(true), _type(object_type::actor), _team(team::none), _myColider(box),_rotation(0)
{
}

