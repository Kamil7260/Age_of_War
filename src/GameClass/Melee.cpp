#include "Melee.hpp"
#include "../Core/Application.hpp"
Melee::Melee(const base::collider& collider, int hp, int attack, float speedAttack, float speedMove)
	:Mob(hp,attack,speedAttack,speedMove), _touchEnemy(false), _isCollided(false), _enableAttack(false)
{
	_animationSpeed = 0.05f;
	_timer = 0.f;
	_myColider = { 10.f,10.f,25.f,25.f };
	_speedMove = 25.f;	
}

Melee::Melee(Melee& source)
	:_timer(source._timer), _attackClip(source._attackClip),
	_idleClip(source._idleClip), _walkClip(source._walkClip),
	_dieClip(source._dieClip), _touchEnemy(source._touchEnemy),
	_isCollided(source._isCollided), _enableAttack(source._enableAttack)
{
}

Melee::Melee(Melee&& source) noexcept
	:_timer(source._timer), _attackClip(source._attackClip),
	_idleClip(source._idleClip), _walkClip(source._walkClip),
	_dieClip(source._dieClip), _touchEnemy(source._touchEnemy),
	_isCollided(source._isCollided), _enableAttack(source._enableAttack)
{
	_attackClip.clear();
	 _idleClip.clear();
	_walkClip.clear();
	_dieClip.clear();
}

Melee& Melee::operator=(Melee& source)
{
	_timer = source._timer;
	_attackClip = source._attackClip;
	_idleClip = source._idleClip;
	_walkClip = source._walkClip;
	_dieClip = source._dieClip;
	_touchEnemy = source._touchEnemy;
	_isCollided = source._isCollided;
	_enableAttack = source._enableAttack;
	return *this;
}

Melee& Melee::operator=(Melee&& source) noexcept
{
	_timer = source._timer;
	_attackClip = source._attackClip;
	_idleClip = source._idleClip;
	_walkClip = source._walkClip;
	_dieClip = source._dieClip;
	_touchEnemy = source._touchEnemy;
	_isCollided = source._isCollided;
	_enableAttack = source._enableAttack;
	_attackClip.clear();
	_idleClip.clear();
	_walkClip.clear();
	_dieClip.clear();
	return *this;
}

void Melee::setAnimatorName(const char* name)
{
	_walkClip = name;
	_walkClip += "_walk";
	addClip(core::Application::getInstance().getClip(_walkClip.c_str()), _walkClip);

	_idleClip = name;
	_idleClip += "_idle";
	addClip(core::Application::getInstance().getClip(_idleClip.c_str()), _idleClip);

	_dieClip = name;
	_dieClip += "_die";
	addClip(core::Application::getInstance().getClip(_dieClip.c_str()), _dieClip);
	auto d = _container.find(_dieClip);
	d->second.setCallback([&]()->void {
		_isActive = false;
		core::Renderer::getInstance().clearNoActive();
		});

	_attackClip = name;
	_attackClip += "_attack";
	addClip(core::Application::getInstance().getClip(_attackClip.c_str()), _attackClip);
	
	auto k = _container.find(_attackClip);
	k->second.setCallback([&]()->void {
		_enableAttack = true;
		});
}

void Melee::onUpdate()
{
	float delta = core::Application::getInstance().getTime();
	_timer += delta;
	if (_hp > 0)
	{
		if (!_isCollided)
		{
			if (!_isRunning)
			{
				play(_walkClip.c_str());
			}
			move(sf::Vector2f(_speedMove * delta, 0.f));
		}
		else if (!_isRunning) {
			if (!_touchEnemy)
				play(_idleClip.c_str());
			else {
				play(_attackClip.c_str());
			}
		}
	}
	_touchEnemy = false;
	_isCollided = false;
	updateAnimator();
}

void Melee::damage(int dmg)
{
	Mob::damage(dmg);
	if (_hp <= 0)
	{
		if(_currentClipName != _dieClip)
		play(_dieClip.c_str());
	}
}

void Melee::setPosition(const sf::Vector2f& pos)
{
	_position = pos;
	_sprite->setPosition(pos);
}

void Melee::onCollision(std::unique_ptr<base::Actor>& collision)
{
	if (collision->getTeam() != _team)
	{
		if (_enableAttack && collision->getTag() == "Mob")
		{
			_enableAttack = false;
			auto ptr = static_cast<base::Mob*>(collision.get());
			ptr->damage(_attack);
			return;
		}
		_touchEnemy = true;
		_isCollided = true;
		if (_currentClipName != _attackClip && _hp>0)
		{
			play(_attackClip.c_str());
			return;
		}
	}

	if (_hp <= 0)
		return;

	if (_speedMove > 0)
	{
		if (collision->getPosition().x > _position.x)
		{
			if(_currentClipName != _attackClip)
			play("caveman_idle");
			_isCollided = true;
		}
		return;
	}
	else{
		if (collision->getPosition().x < _position.x)
		{
			if (_currentClipName != _attackClip)
			play("caveman_idle");
			_isCollided = true;
		}
		return;
	}
}


