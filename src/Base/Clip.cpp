#include "Clip.hpp"
#include "../Core/Application.hpp"

base::Clip::Clip(float speed)
	: _isFinish(true), _speed(speed), _curTime(0.f)
{
}

base::Clip::Clip(Clip& source)
	: _isFinish(true), _speed(source._speed),
	_curTime(source._curTime), _container(source._container)
{
}

base::Clip::Clip(Clip&& source) noexcept
	:_isFinish(true), _speed(source._speed),
	_curTime(source._curTime),_container(source._container)
{
	source._speed = 0.f;
	source._curTime = 0.f;
	source._container.clear();
}

base::Clip& base::Clip::operator=(Clip& source)
{
	_isFinish = source._isFinish;
	_speed = source._speed;
	_curTime = source._curTime;
	_container = source._container;
	return *this;
}

base::Clip& base::Clip::operator=(Clip&& source) noexcept
{
	_isFinish = source._isFinish;
	_speed = source._speed;
	_curTime = source._curTime;
	_container = source._container;
	source._speed = 0.f;
	source._curTime = 0.f;
	source._container.clear();
	return *this;
}

void base::Clip::addFrame(const std::shared_ptr<sf::Texture>& tex)
{
	_container.push_back(tex);
}

bool base::Clip::update()
{
	_curTime += core::Application::getInstance().getTime();
	if (_curTime >= _speed)
	{
		_curTime = 0.f;
		++_currentFrame;
		if (_currentFrame == _container.end())
		{
			_isFinish = true;
			return false;
		}
		_sprite->setTexture(*(*_currentFrame));
	}
	return true;
}

void base::Clip::setSprite(const std::shared_ptr<sf::Sprite>& sprite)
{
	_sprite = sprite;
}

void base::Clip::start()
{
	_isFinish = false;
	_currentFrame = _container.begin();
	_sprite->setTexture(*(*_currentFrame));
	_curTime = 0.f;
}
