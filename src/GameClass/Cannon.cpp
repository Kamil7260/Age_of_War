#include <cmath>

#include "Cannon.hpp"
#include "../Core/Renderer.hpp"
#include "../Core/Application.hpp"
#include "Bullet.hpp"

Cannon::Cannon(int maxDMG, int minDMG, int range, float reloadTime,float bulletspeed, const float deltaBulletPos, const float fireSpeed,const int sellPrice)
	:_maxDMG(maxDMG), _minDMG(minDMG), _range(range), _reloadTime(reloadTime), _curTime(0.f), _enableShot(true),
	_bulletSpeed(bulletspeed), _rangeSpawn(deltaBulletPos), _longRange(false), _fireSpeed(fireSpeed), _sellPrice(sellPrice)
{
	_team = base::team::player;
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
				play(_currentClipName);
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
	_currentClipName = name;
	clip.setCallbackOnTime([&]()->void {
		_enableShot = true;

		auto r = _sprite->getRotation();
		auto x = std::cos(r * 3.14f / 180.f);
		auto y = std::sin(r * 3.14f / 180.f);
		if (_longRange) {
			y += 0.05f;
			x -= 0.05f;
		}
		std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>(_minDMG,_maxDMG,_bulletSpeed, sf::Vector2f(x,y));
		bullet->setRotation(static_cast<int>(_sprite->getRotation()));
		if (_longRange)
		{
			r+=100;
			r = 180 - r;
			x = std::cos(r * 3.14f / 180.f) * _rangeSpawn;
			y = std::sin(r * 3.14f / 180.f) * _rangeSpawn;
		}
		else {
			x = 0.f;
			y = 0.f;
		}
		_bulletSpawnPoint = { _position.x - x, _position.y - y };
		bullet->setPosition(_bulletSpawnPoint);
		bullet->setTexture(*_bulletTex);
		bullet->setTeam(_team);
		core::Renderer::getInstance().addBullet(std::move(bullet));
		}, _fireSpeed);
	_container.insert(std::make_pair(name, std::move(clip)));
}

void Cannon::correctDirection(std::unique_ptr<base::Actor>& target)
{
	auto k = target->getPosition();
	float y = _position.y - k.y;
	float z = k.x - _position.x;
	z = std::sqrt(y * y + z * z);
	float val =  std::acos(y/z) * 180.f / 3.14f;
	if (_longRange)
	{
		auto angle = -(90 - val);
		if(angle>15)
			_sprite->setRotation(15);
		else {
			_sprite->setRotation(angle);
		}
		return;
	}
	_sprite->setRotation(-(90 - val));
}
