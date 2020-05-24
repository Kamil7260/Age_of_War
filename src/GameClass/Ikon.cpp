#include "Ikon.hpp"

#include "../Core/Application.hpp"

Ikon::Ikon(float time)
	:_maxTime(time), _timer(0.f)
{
	_shadow.setFillColor(sf::Color::Black);
	_shadow.setFillColor(sf::Color(0, 0, 0, 120));
}


Ikon::Ikon(const Ikon& source)
	: _sprite(source._sprite), _maxTime(source._maxTime), _timer(source._timer),_shadow(source._shadow), _onEnd(source._onEnd), _shadowSize(source._shadowSize)
{
}

Ikon::Ikon(Ikon&& source) noexcept
	: _sprite(source._sprite), _maxTime(source._maxTime), _timer(source._timer), _shadow(source._shadow), _onEnd(source._onEnd), _shadowSize(source._shadowSize)
{
	source._onEnd = nullptr;
}

Ikon& Ikon::operator=(const Ikon& source)
{
	_maxTime = source._maxTime;
	_timer = source._timer;
	_sprite = source._sprite;
	_shadow = source._shadow;
	_onEnd = source._onEnd;
	_shadowSize = source._shadowSize;
	return *this;
}

Ikon& Ikon::operator=(Ikon&& source) noexcept
{
	_maxTime = source._maxTime;
	_timer = source._timer;
	_sprite = source._sprite;
	_shadow = source._shadow;
	_onEnd = source._onEnd;
	_shadowSize = source._shadowSize;
	source._onEnd = nullptr;
	return *this;
}

void Ikon::onUpdate()
{
	float delta = core::Application::getInstance().getTime();
	_timer += delta;

	if(_timer>_maxTime)
		if (_onEnd != nullptr)
		{
			_onEnd();
		}
	_shadow.setSize(sf::Vector2f(_shadow.getSize().x, _shadowSize.y * (_maxTime-_timer)/_maxTime));
}

void Ikon::setPosition(const sf::Vector2f& pos)
{
	_sprite.setPosition(pos);
	_shadow.setPosition(pos);
	_position = pos;
}

void Ikon::setScale(const sf::Vector2f& sca)
{
	_sprite.setScale(sca);
	_shadow.setScale(sca);
	_scale = sca;
}

void Ikon::move(const sf::Vector2f& delta)
{
	_sprite.move(delta);
	_shadow.move(delta);
	_position += delta;
}

void Ikon::setTexture(const sf::Texture& tex)
{
	_sprite.setTexture(tex);
	_shadowSize = sf::Vector2f(tex.getSize());
	_shadow.setSize(_shadowSize);
}

void Ikon::setTimePoint(const float points)
{
	_maxTime = points;
}

void Ikon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
	target.draw(_shadow, states);
}

void Ikon::setCallBackOnEnd(const std::function<void()>& onEnd)
{
	_onEnd = onEnd;
}
