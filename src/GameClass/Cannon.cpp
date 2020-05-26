#include <cmath>

#include "Cannon.hpp"
#include "../Core/Renderer.hpp"
#include "../Core/Application.hpp"
#include "Bullet.hpp"

Cannon::Cannon(int maxDMG, int minDMG, int range, float reloadTime,float bulletspeed)
	:_maxDMG(maxDMG), _minDMG(minDMG), _range(range), _reloadTime(reloadTime), _curTime(0.f), _enableShot(true), _bulletSpeed(bulletspeed)
{
	_team = base::team::player;
}

Cannon::Cannon(const Cannon& source)
	:_maxDMG(source._maxDMG), _minDMG(source._minDMG), _range(source._range), _reloadTime(source._reloadTime), _curTime(source._curTime),
	_enableShot(source._enableShot), _bulletTex(source._bulletTex), _bulletSpeed(source._bulletSpeed)
{
}

Cannon::Cannon(Cannon&& source) noexcept
	:_maxDMG(source._maxDMG), _minDMG(source._minDMG), _range(source._range), _reloadTime(source._reloadTime), _curTime(source._curTime),
	_enableShot(source._enableShot), _bulletTex(source._bulletTex), _bulletSpeed(source._bulletSpeed)
{
}

Cannon& Cannon::operator=(const Cannon& source)
{
	_bulletSpeed = source._bulletSpeed;
	_maxDMG = source._maxDMG;
	_minDMG = source._minDMG;
	_range = source._range;
	_reloadTime = source._reloadTime;
	_curTime = source._curTime;
	_bulletTex = source._bulletTex;
	_enableShot = source._enableShot;
	return *this;
}

Cannon& Cannon::operator=(Cannon&& source) noexcept
{
	_bulletSpeed = source._bulletSpeed;
	_maxDMG = source._maxDMG;
	_minDMG = source._minDMG;
	_range = source._range;
	_reloadTime = source._reloadTime;
	_curTime = source._curTime;
	_bulletTex = source._bulletTex;
	_enableShot = source._enableShot;
	return *this;
}

void Cannon::onUpdate()
{
	updateAnimator();
	auto& k = core::Renderer::getInstance().isEnemyInRange(_position, static_cast<float>(_range), _team);
	
	if (k != nullptr)
	{
		correctDirection(k);
		if (!_isRunning)
		{
			_curTime += core::Application::getInstance().getTime();
			if (_curTime > _reloadTime)
			{
				play("IC1");
				_curTime = 0.f;
			}
		}
	}
	else {
		_curTime = 0.f;
	}
}

void Cannon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*_sprite, states);
}

void Cannon::setBulletTexture(const std::shared_ptr<sf::Texture>& tex)
{
	_bulletTex = tex;
}

void Cannon::addClip(base::Clip clip, const std::string& name)
{
	_sprite->setTexture(*clip.getMask());
	_sprite->setOrigin(clip.getOriginMask());
	clip.setSprite(_sprite);
	clip.setCallbackOnTime([&]()->void {
		_enableShot = true;
		auto x = std::cos(_sprite->getRotation() * 3.14f / 180.f);
		auto y = std::sin(_sprite->getRotation() * 3.14f / 180.f);
		std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>(_minDMG,_maxDMG,_bulletSpeed, sf::Vector2f(x,y));
		bullet->setPosition(_position);
		bullet->setTexture(*_bulletTex);
		bullet->setTeam(_team);
		core::Renderer::getInstance().addBullet(std::move(bullet));
		}, 3.f);
	_container.insert(std::make_pair(name, std::move(clip)));
}

void Cannon::correctDirection(std::unique_ptr<base::Actor>& target)
{
	auto k = target->getPosition();
	float y = _position.y - k.y;
	float z = k.x - _position.x;
	z = std::sqrt(y * y + z * z);
	float val =  std::acos(y/z) * 180.f / 3.14f;
	_sprite->setRotation(-(90-val));
}
