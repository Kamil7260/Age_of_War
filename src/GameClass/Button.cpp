#include "Button.hpp"

Button::Button(const base::collider& collider)
	:Actor(collider), _onClickEvent(nullptr)
{
}

Button::Button(const Button& source)
	:_sprite(source._sprite), _onClickEvent(source._onClickEvent)
{
}

Button::Button(Button&& source) noexcept
	:_sprite(source._sprite), _onClickEvent(source._onClickEvent)
{
	source._onClickEvent = nullptr;
}

void Button::setTexture(const sf::Texture& tex)
{
	_sprite.setTexture(tex);
}

Button& Button::operator=(const Button& source)
{
	_onClickEvent = source._onClickEvent;
	_sprite = source._sprite;
	return *this;
}

Button& Button::operator=(Button&& source) noexcept
{
	_onClickEvent = source._onClickEvent;
	_sprite = source._sprite;
	source._onClickEvent = nullptr;
	return *this;
}

void Button::onUpdate()
{
}

void Button::onMouseCollision(bool isPressed)
{
	if (_onClickEvent != nullptr)
	{
		_onClickEvent(isPressed);
	}
}

void Button::setPosition(const sf::Vector2f& pos)
{
	_sprite.setPosition(pos);
	_position = pos;
}

void Button::setScale(const sf::Vector2f& sca)
{
	_sprite.setScale(sca);
	_scale = sca;
}

void Button::move(const sf::Vector2f& delta)
{
	_sprite.move(delta);
	_position += delta;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
}

void Button::setClickEvent(const std::function<void(bool)>& func)
{
	_onClickEvent = func;
}
