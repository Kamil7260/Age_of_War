#include <random>

#include "Melee.hpp"
#include "../Core/Application.hpp"

Melee::Melee(const base::collider& collider, int hp, int attack, int maxAttack, float speedAttack, float speedMove)
	:Mob(hp, attack,maxAttack, speedAttack, speedMove),_timer(0.f),_attackTimer(0.f), _touchEnemy(false), _isCollided(false), _enableAttack(true), _died(false)
{
	_myColider = collider;
}

Melee::Melee(Melee& source)
	:_timer(source._timer),_attackTimer(source._attackTimer), _attackClip(source._attackClip),
	_idleClip(source._idleClip), _walkClip(source._walkClip),
	_dieClip(source._dieClip), _touchEnemy(source._touchEnemy),
	_isCollided(source._isCollided), _enableAttack(source._enableAttack),
	_died(source._died)
{
}

Melee::Melee(Melee&& source) noexcept
	:_timer(source._timer),_attackTimer(source._attackTimer), _attackClip(source._attackClip),
	_idleClip(source._idleClip), _walkClip(source._walkClip),
	_dieClip(source._dieClip), _touchEnemy(source._touchEnemy),
	_isCollided(source._isCollided), _enableAttack(source._enableAttack),
	_died(source._died)
{
	_attackClip.clear();
	 _idleClip.clear();
	_walkClip.clear();
	_dieClip.clear();
}

Melee& Melee::operator=(Melee& source)
{
	_attackTimer = source._attackTimer;
	_timer = source._timer;
	_attackClip = source._attackClip;
	_idleClip = source._idleClip;
	_walkClip = source._walkClip;
	_dieClip = source._dieClip;
	_touchEnemy = source._touchEnemy;
	_isCollided = source._isCollided;
	_enableAttack = source._enableAttack;
	_died = source._died;
	return *this;
}

Melee& Melee::operator=(Melee&& source) noexcept
{
	_timer = source._timer;
	_attackTimer = source._attackTimer;
	_attackClip = source._attackClip;
	_idleClip = source._idleClip;
	_walkClip = source._walkClip;
	_dieClip = source._dieClip;
	_touchEnemy = source._touchEnemy;
	_isCollided = source._isCollided;
	_enableAttack = source._enableAttack;
	_died = source._died;
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
		_died = true;
		_timer = 0.f;
	//	_activeCollider = false;
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
		if (_currentClipName == _attackClip && _enableAttack)
		{
			_attackTimer += delta;
		}
		else _attackTimer = 0;

		if (!_isCollided)
		{
			if (!_isRunning || _currentClipName != _walkClip)
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
	else{
		if (_died && _timer > 2.f)
		{
			_isActive = false;
			core::Renderer::getInstance().clearNoActive();
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
		_position = { 0.f,0.f };
		_activeCollider = false;
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
		_touchEnemy = true;
		_isCollided = true;
		if (_attackTimer>_speedAttack && collision->getTag() == "Mob")
		{
			_attackTimer = 0.f;
			_enableAttack = false;
			if (_hp <= 0) return;
			std::random_device mch;
			std::default_random_engine generator(mch());
			std::uniform_int_distribution<int> distribution(_attack,_maxAttack);
			int attack_roll = distribution(generator);

			auto ptr = static_cast<base::Mob*>(collision.get());
			ptr->damage(attack_roll);
			return;
		}
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
		if (collision->getPosition().x >= _position.x)
		{
			if(_currentClipName != _attackClip)
			play(_idleClip.c_str());
			_isCollided = true;
		}
		return;
	}
	else{
		if (collision->getPosition().x <= _position.x)
		{
			if (_currentClipName != _attackClip)
			play(_idleClip.c_str());
			_isCollided = true;
		}
		return;
	}
}


