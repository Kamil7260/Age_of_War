#include "Actor.hpp"

base::Actor::Actor()
	:_isActive(true), _type(object_type::actor), _team(team::none), _myColider({1.f,1.f,1.f,1.f})
{
}

base::Actor::Actor(const Actor& source)
	:_tag(source._tag),_isActive(source._isActive), _type(source._type),
	_team(source._team), _position(source._position),
	_scale(source._scale), _myColider(source._myColider)
{
}

base::Actor::Actor(Actor&& source) noexcept
	:_tag(source._tag),_isActive(source._isActive), _type(source._type),
	_team(source._team), _position(source._position),
	_scale(source._scale),_myColider(source._myColider)
{
}

base::Actor& base::Actor::operator=(const Actor& source)
{
	_tag = source._tag;
	_isActive = source._isActive;
	_type = source._type;
	_team = source._team;
	_position = source._position;
	_scale = source._scale;
	_myColider = source._myColider;
	return *this;
}

base::Actor& base::Actor::operator=(Actor&& source) noexcept
{
	_tag = source._tag;
	_isActive = source._isActive;
	_type = source._type;
	_team = source._team;
	_position = source._position;
	_scale = source._scale;
	return *this;
}
