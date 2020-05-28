#include "Clip.hpp"
#include "../Core/Application.hpp"

base::Clip::Clip(float speed)
	: _isFinish(true), _speed(speed), _curTime(0.f), _origin(0.5f, 0.5f), _callback(nullptr), _callbackOnTime(nullptr), _onTime(0.f), _onTimeCalled(false)
{
}

void base::Clip::addFrame(const std::shared_ptr<sf::Texture>& tex)
{
	_container.push_back(tex);
}

bool base::Clip::update()
{
	auto delta = core::Application::getInstance().getTime();
	_curTime += delta;
	if (_onTime > 0.f)
	{
		_onTime -= delta;
	}
	else if(!_onTimeCalled) {
		if (_callbackOnTime != nullptr) {
			_callbackOnTime();
			_onTimeCalled = true;
		}
	}
	if (_curTime >= _speed)
	{
		_curTime = 0.f;
		++_currentFrame;
		if (_currentFrame == _container.end())
		{
			_currentFrame = _container.begin();
			_isFinish = true;
			if (_callback != nullptr)
				_callback();
			return true;
		}
		_sprite->setTexture(*(*_currentFrame));
	}
	return false;
}

void base::Clip::setOrigin(const sf::Vector2f& origin)
{
	_origin = origin;
}

void base::Clip::setSprite(const std::shared_ptr<sf::Sprite>& sprite)
{
	_sprite = sprite;
}

void base::Clip::setSpeed(const float speed)
{
	_speed = speed;
}

void base::Clip::start()
{
	_onTimeCalled = false;
	_isFinish = false;
	_currentFrame = _container.begin();
	_sprite->setTexture(*(*_currentFrame),true);
	_curTime = 0.f;
	_onTime = 0.f;
	_sprite->setOrigin(_origin);
}

const sf::Vector2f& base::Clip::getOriginMask() const
{
	return _origin;
}

void base::Clip::setCallbackOnTime(const std::function<void()>& callme, const float time)
{
	_callbackOnTime = callme;
	_onTime = time;
}

std::shared_ptr<sf::Texture> base::Clip::getMask() const
{
	if (_container.empty())
		return nullptr;
	return *_container.begin();
}

void base::Clip::setCallback(const std::function<void()>& callme)
{
	_callback = callme;
}
