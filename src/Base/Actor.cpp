#include "Actor.hpp"

base::Actor::Actor()
	:_isActive(true), _type(object_type::actor), _team(team::none) 
{
}

base::Actor::Actor(const Actor& source)
	:_isActive(source._isActive), _type(source._type),
	_team(source._team), _position(source._position),
	_scale(source._scale)
{
}

base::Actor::Actor(Actor&& source) noexcept
	:_isActive(source._isActive), _type(source._type),
	_team(source._team), _position(source._position),
	_scale(source._scale)
{
}

base::Actor& base::Actor::operator=(const Actor& source)
{
	_isActive = source._isActive;
	_type = source._type;
	_team = source._team;
	_position = source._position;
	_scale = source._scale;
	return *this;
}

base::Actor& base::Actor::operator=(Actor&& source) noexcept
{
	_isActive = source._isActive;
	_type = source._type;
	_team = source._team;
	_position = source._position;
	_scale = source._scale;
	return *this;
}
