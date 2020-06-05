#include "Counter.hpp"

#include "../Core/Application.hpp"

Counter::Counter(int* target)
	:_target(target), _curTime(0.f), _currentValue(0)
{
}

void Counter::setFont(const sf::Font& font)
{
	_text.setFont(font);
}

void Counter::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*_sprite, states);
	target.draw(_text, states);
}

void Counter::setScale(const sf::Vector2f& sca)
{
	_sprite->setScale(sca);
	_text.setScale(sca);
	_scale = sca;
}

void Counter::move(const sf::Vector2f& delta)
{
	_sprite->move(delta);
	_text.move(delta);
	_position += delta;
}

void Counter::setPosition(const sf::Vector2f& pos)
{
	_sprite->setPosition(pos);
	_text.setPosition(pos + sf::Vector2f(20.f,-15.f));
	_position = pos;
}

void Counter::setColor(const sf::Color& color)
{
	_text.setFillColor(color);
}

void Counter::onUpdate()
{
	updateAnimator();
	if (!_isRunning)
		play(_currentClipName);

	_curTime += core::Application::getInstance().getTime();
	if(*_target != _currentValue)
	if (_curTime > 0.01f) {
		_curTime = 0.f;
		if (_currentValue < *_target)
		{
			++_currentValue;
		}
		else {
			--_currentValue;
		}
		_text.setString(std::to_string(_currentValue));
	}
}

void Counter::addClip(base::Clip clip, const std::string& name)
{
	clip.setSprite(_sprite);
	_container.insert(std::make_pair(name, std::move(clip)));
	_currentClipName = name;
}

void Counter::setCharacterSize(const unsigned int size)
{
	_text.setCharacterSize(size);
}
