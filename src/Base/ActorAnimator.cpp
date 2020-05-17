#include "ActorAnimator.hpp"

#include "../Logger/Logger.hpp"

base::ActorAnimator::ActorAnimator()
	:_isRunning(false),_isFinish(true)
{
	_sprite = std::make_shared<sf::Sprite>();
}

base::ActorAnimator::ActorAnimator(const ActorAnimator& source)
	:_isRunning(source._isRunning),_container(source._container), _currentClip(source._currentClip),_isFinish(source._isFinish)
{
	_sprite = std::make_shared<sf::Sprite>();
}

base::ActorAnimator::ActorAnimator(ActorAnimator&& source) noexcept
	:_isRunning(source._isRunning), _container(source._container), _currentClip(source._currentClip),_isFinish(source._isFinish)
{
	_sprite = std::make_shared<sf::Sprite>();
	source._container.clear();
}

base::ActorAnimator& base::ActorAnimator::operator=(const ActorAnimator& source)
{
	_isRunning = source._isRunning;
	_isFinish = source._isFinish;
	_container = source._container;
	_sprite = std::make_shared<sf::Sprite>();
	_currentClip = source._currentClip;
	return *this;
}

base::ActorAnimator& base::ActorAnimator::operator=(ActorAnimator&& source) noexcept
{
	_isRunning = source._isRunning;
	_isFinish = source._isFinish;
	_sprite = std::make_shared<sf::Sprite>();
	_container = source._container;
	_currentClip = source._currentClip;
	source._container.clear();
	return *this;
}

bool base::ActorAnimator::play(const char* key)
{
	_currentClip = _container.find(key);
	if (_currentClip == _container.end())
	{
		LOG_ERROR("Can not find Clip with key : ", key);
		_isRunning = false;
		return _isRunning;
	}
	_isRunning = true;
	return _isRunning;
}

void base::ActorAnimator::addClip(std::shared_ptr<base::Clip>& clip, const std::string& name)
{
	clip->setSprite(_sprite);
	_container.insert(std::make_pair(name, std::move(clip)));
}

void base::ActorAnimator::updateAnimator()
{
	if(_isRunning)
		_isFinish = _currentClip->second->update();

	onUpdate();
}

void base::ActorAnimator::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(isActive())
	target.draw(*_sprite, states);
}

void base::ActorAnimator::setPosition(const sf::Vector2f& pos)
{
	_sprite->setPosition(pos);
}

void base::ActorAnimator::setScale(const sf::Vector2f& sca)
{
	_sprite->setScale(sca);
}

void base::ActorAnimator::move(const sf::Vector2f& delta)
{
	_sprite->move(delta);
}
