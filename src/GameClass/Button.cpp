#include "Button.hpp"

Button::Button(const base::collider& collider)
	:Actor(collider), _onClickEvent(nullptr),_shadowDraw(false), _isCollided(false), _isReleaseCalled(true),_drawBox(false)
{
	_shadow.setFillColor(sf::Color::White);
	_shadow.setFillColor(sf::Color(255, 255, 255, 60));
}

void Button::setTexture(const sf::Texture& tex)
{
	_sprite.setTexture(tex);
	_shadow.setSize(sf::Vector2f(tex.getSize()));
}

void Button::onUpdate()
{
	if(!_shadowDraw)
		_shadow.setFillColor(sf::Color(255, 255, 255, 0));
	_shadowDraw = false;

	if (_isCollided)
	{
		_drawBox = true;
	}
	else {
		_drawBox = false;
		if (!_isReleaseCalled)
		{
			if(_onReleaseEvent != nullptr)
				_onReleaseEvent();
			_isReleaseCalled = true;
		}
	}
	_isCollided = false;
}

void Button::onMouseCollision(bool isPressed)
{
	_infoBox.refresh();
	_isCollided = true;
	_shadowDraw = true;
	_isReleaseCalled = false;
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

Info& Button::getInfo()
{
	return _infoBox;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
	target.draw(_shadow, states);
	if (_drawBox)
	{
		_infoBox.draw(target,states);
	}
}

void Button::setCallbackOnMouseRelease(const std::function<void()>& func)
{
	_onReleaseEvent = func;
}

void Button::setClickEvent(const std::function<void(bool)>& func)
{
	_onClickEvent = func;
}
