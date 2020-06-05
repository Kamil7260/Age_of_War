#include <random>
#include "../Core/Application.hpp"
#include "../Core/ResourceManager.hpp"
#include "../Base/Stronghold.hpp"

#include "Range.hpp"
#include "TextDisplay.hpp"

Range::Range(const base::collider& collider, int hp, int attack, int maxAttack, int range, float speedAttack, float speedMove, int income)
	:Mob(hp, attack, maxAttack, speedAttack, speedMove, income), _timer(0.f), _attackTimer(0.f), _range(range),
	_touchEnemy(false), _isCollided(false), _enableAttack(true), _died(false), _inRange(false)
{
	_myColider = collider;
}

void Range::setAnimatorName(const std::string& name)
{
	_walkClip = name;
	_walkClip += "_walk";
	addClip(core::Application::getInstance().getClip(_walkClip), _walkClip);

	_idleClip = name;
	_idleClip += "_idle";
	addClip(core::Application::getInstance().getClip(_idleClip), _idleClip);

	_dieClip = name;
	_dieClip += "_die";
	addClip(core::Application::getInstance().getClip(_dieClip), _dieClip);
	auto d = _container.find(_dieClip);
	d->second.setCallback([&]()->void {
		_died = true;
		_timer = 0.f;
		});

	_attackClip = name;
	_attackClip += "_attack";
	addClip(core::Application::getInstance().getClip(_attackClip), _attackClip);

	auto k = _container.find(_attackClip);
	k->second.setCallback([&]()->void {
		_enableAttack = true;
		});

	_idleShotClip = name;
	_idleShotClip += "_idleshot";
	addClip(core::Application::getInstance().getClip(_idleShotClip), _idleShotClip);
	k = _container.find(_idleShotClip);
	k->second.setCallback([&]()->void {
		_enableAttack = true;
		});

	_walkShotClip = name;
	_walkShotClip += "_walkshot";
	addClip(core::Application::getInstance().getClip(_walkShotClip), _walkShotClip);
	k = _container.find(_walkShotClip);
	k->second.setCallback([&]()->void {
		_enableAttack = true;
		});
}

void Range::onUpdate()
{
	float delta = core::Application::getInstance().getTime();
	_timer += delta;
	if (_hp > 0)
	{
		auto &inRangeActor = core::Renderer::getInstance().isEnemyInRange(_position, 250.f, _team);
		if (inRangeActor  != nullptr)
		{
			_inRange = true;
			if (_attackTimer> _speedAttack) {
				_attackTimer = 0.f;
				_enableAttack = false;
				if (inRangeActor->getTag() == "Mob")
				{
					std::random_device mch;
					std::default_random_engine generator(mch());
					std::uniform_int_distribution<int> distribution(_attack, _maxAttack);
					int attack_roll = distribution(generator);
					auto ptr = static_cast<base::Mob*>(inRangeActor.get());
					ptr->damage(attack_roll);
				}
			}
		}
		else _inRange = false;

		if ((_currentClipName == _attackClip || _currentClipName == _idleShotClip || _currentClipName == _walkShotClip) && _enableAttack)
		{
			_attackTimer += delta;
		}
		else _attackTimer = 0;

		if (!_isCollided)
		{
			if (!_isRunning || ( _currentClipName != _walkClip && _currentClipName != _walkShotClip))
			{
				if(!_inRange)
					play(_walkClip);
				else 
					play(_walkShotClip);
			}
			move(sf::Vector2f(_speedMove * delta, 0.f));
		}
		else if (!_isRunning) {
			if (!_touchEnemy)
			{
				if (!_inRange)
					play(_idleClip);
				else
					play(_idleShotClip);
			}
			else {
				play(_attackClip);
			}
		}
	}
	else {
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


void Range::damage(int dmg)
{
	Mob::damage(dmg);
	if (_hp <= 0)
	{
		if (_team != base::team::player)
		{
			auto ptr = std::make_unique<TextDisplay>(2.5f, 70.f);
			ptr->setPosition(_position);
			ptr->setCharacterSize(20);
			ptr->setFont(*core::ResourceManager<sf::Font>::getInstance().get("Assets/fonts/3.ttf"));
			ptr->setIncome(_income);
			ptr->setColor(sf::Color::Yellow);
			ptr->setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/other/5.png"));
			core::Renderer::getInstance().addObject(std::move(ptr), base::object_type::gui);
		}

		_position = { 0.f,0.f };
		_activeCollider = false;
		if (_currentClipName != _dieClip)
			play(_dieClip);
	}
}

void Range::setPosition(const sf::Vector2f& pos)
{
	_position = pos;
	_sprite->setPosition(pos);
}

void Range::onCollision(std::unique_ptr<base::Actor>& collision)
{
	if (collision->getTag() == "Bullet")
		return;

	if (collision->getTeam() != _team)
	{
		_touchEnemy = true;
		_isCollided = true;
		if (_attackTimer > _speedAttack && collision->getTag() == "Mob")
		{
			_attackTimer = 0.f;
			_enableAttack = false;
			if (_hp <= 0) return;
			std::random_device mch;
			std::default_random_engine generator(mch());
			std::uniform_int_distribution<int> distribution(_attack, _maxAttack);
			int attack_roll = distribution(generator);
			auto ptr = static_cast<base::Mob*>(collision.get());
			ptr->damage(attack_roll/2);

			return;
		}

		if (collision->getTag() == "Player" || collision->getTag() == "Enemy")
		{
			if (_attackTimer > _speedAttack)
			{
				_attackTimer = 0.f;
				_enableAttack = false;
				if (_hp <= 0) return;
				std::random_device mch;
				std::default_random_engine generator(mch());
				std::uniform_int_distribution<int> distribution(_attack, _maxAttack);
				int attack_roll = distribution(generator);

				auto ptr = static_cast<base::Stronghold*>(collision.get());
				ptr->damage(attack_roll);
			}
		}

		if (_currentClipName != _attackClip && _hp > 0)
		{
			play(_attackClip);
			return;
		}
	}

	if (_hp <= 0)
		return;

	if (_speedMove > 0)
	{
		if (collision->getPosition().x >= _position.x)
		{
			if (_currentClipName != _attackClip)
			{
				if(_currentClipName != _idleClip && _currentClipName != _idleShotClip)
				if (!_inRange)
					play(_idleClip);
				else
					play(_idleShotClip);
			}
			_isCollided = true;
		}
		return;
	}
	else {
		if (collision->getPosition().x <= _position.x)
		{
			if (_currentClipName != _attackClip)
			{
				if (_currentClipName != _idleClip && _currentClipName != _idleShotClip)
				if (!_inRange)
					play(_idleClip);
				else
					play(_idleShotClip);
			}
			_isCollided = true;
		}
		return;
	}
}


