#include "Button.hpp"

Button::Button(const base::collider& collider)
	:Actor(collider), _onClickEvent(nullptr),_shadowDraw(false)
{
	_shadow.setFillColor(sf::Color::White);
	_shadow.setFillColor(sf::Color(255, 255, 255, 60));
}

Button::Button(const Button& source)
	:_sprite(source._sprite), _onClickEvent(source._onClickEvent),_shadow(source._shadow), _shadowDraw(source._shadowDraw)
{
}

Button::Button(Button&& source) noexcept
	:_sprite(source._sprite), _onClickEvent(source._onClickEvent), _shadow(source._shadow), _shadowDraw(source._shadowDraw)
{
	source._onClickEvent = nullptr;
}

void Button::setTexture(const sf::Texture& tex)
{
	_sprite.setTexture(tex);
	_shadow.setSize(sf::Vector2f(tex.getSize()));
}

Button& Button::operator=(const Button& source)
{
	_onClickEvent = source._onClickEvent;
	_sprite = source._sprite;
	_shadow = source._shadow;
	_shadowDraw = source._shadowDraw;
	return *this;
}

Button& Button::operator=(Button&& source) noexcept
{
	_onClickEvent = source._onClickEvent;
	_sprite = source._sprite;
	_shadow = source._shadow;
	_shadowDraw = source._shadowDraw;
	source._onClickEvent = nullptr;
	return *this;
}

void Button::onUpdate()
{
	if(!_shadowDraw)
		_shadow.setFillColor(sf::Color(255, 255, 255, 0));
	_shadowDraw = false;
}

void Button::onMouseCollision(bool isPressed)
{
	_shadowDraw = true;
	if (_onClickEvent != nullptr)
	{
		_onClickEvent(isPressed);
	}
	if (isPressed)
	{
		_shadow.setFillColor(sf::Color(255, 255, 255, 140));
		return;
	}

	_shadow.setFillColor(sf::Color(0, 0, 0, 50));
}

void Button::setPosition(const sf::Vector2f& pos)
{
	_shadow.setPosition(pos);
	_sprite.setPosition(pos);
	_position = pos;
}

void Button::setScale(const sf::Vector2f& sca)
{
	_shadow.setScale(sca);
	_sprite.setScale(sca);
	_scale = sca;
}

void Button::move(const sf::Vector2f& delta)
{
	_shadow.move(delta);
	_sprite.move(delta);
	_position += delta;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
	target.draw(_shadow, states);
}

void Button::setClickEvent(const std::function<void(bool)>& func)
{
	_onClickEvent = func;
}
