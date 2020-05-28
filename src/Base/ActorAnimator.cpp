#include "ActorAnimator.hpp"

#include "../Logger/Logger.hpp"

base::ActorAnimator::ActorAnimator(float animationspeed)
	:_isRunning(false),_isFinish(true), _animationSpeed(animationspeed)
{
	_sprite = std::make_shared<sf::Sprite>();
}

bool base::ActorAnimator::play(const std::string& key)
{
	_currentClip = _container.find(key);
	if (_currentClip == _container.end())
	{
		LOG_ERROR("Can not find Clip with key : ", key);
		_isRunning = false;
		return _isRunning;
	}
	_currentClipName = key;
	_currentClip->second.setSpeed(_animationSpeed);
	_currentClip->second.start();
	_isRunning = true;
	return _isRunning;
}

void base::ActorAnimator::addClip(base::Clip clip, const std::string& name)
{
	clip.setSprite(_sprite);
	_container.insert(std::make_pair(name, std::move(clip)));
}

void base::ActorAnimator::updateAnimator()
{
	if (_isRunning)
	{
		_isFinish = _currentClip->second.update();
		if (_isFinish)
			_isRunning = false;
	}
}

void base::ActorAnimator::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(isActive())
	target.draw(*_sprite, states);
}

void base::ActorAnimator::setPosition(const sf::Vector2f& pos)
{
	_position = pos;
	_sprite->setPosition(pos);
}

void base::ActorAnimator::setScale(const sf::Vector2f& sca)
{
	_scale = sca;
	_sprite->setScale(sca);
}

void base::ActorAnimator::move(const sf::Vector2f& delta)
{
	_position += delta;
	_sprite->move(delta);
}

void base::ActorAnimator::setAnimationSpeed(const float speed)
{
	_animationSpeed = speed;
}
