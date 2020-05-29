#include "CoinCounter.hpp"

#include "../Core/Application.hpp"

CoinCounter::CoinCounter(int* target)
	:_target(target), _curTime(0.f), _currentCoin(0)
{
	auto k = core::Application::getInstance().getClip("coin");
	addClip(std::move(k), "coin");
	play("coin");
}

void CoinCounter::setFont(const sf::Font& font)
{
	_text.setFont(font);
}

void CoinCounter::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*_sprite, states);
	target.draw(_text, states);
}

void CoinCounter::setScale(const sf::Vector2f& sca)
{
	_sprite->setScale(sca);
	_text.setScale(sca);
	_scale = sca;
}

void CoinCounter::move(const sf::Vector2f& delta)
{
	_sprite->move(delta);
	_text.move(delta);
	_position += delta;
}

void CoinCounter::setPosition(const sf::Vector2f& pos)
{
	_sprite->setPosition(pos);
	_text.setPosition(pos + sf::Vector2f(20.f,-15.f));
	_position = pos;
}

void CoinCounter::setColor(const sf::Color& color)
{
	_text.setFillColor(color);
}

void CoinCounter::onUpdate()
{
	updateAnimator();
	if (!_isRunning)
		play("coin");

	_curTime += core::Application::getInstance().getTime();
	if(*_target != _currentCoin)
	if (_curTime > 0.01f) {
		_curTime = 0.f;
		if (_currentCoin < *_target)
		{
			++_currentCoin;
		}
		else {
			--_currentCoin;
		}
		_text.setString(std::to_string(_currentCoin));
	}
}

void CoinCounter::setCharacterSize(const unsigned int size)
{
	_text.setCharacterSize(size);
}
