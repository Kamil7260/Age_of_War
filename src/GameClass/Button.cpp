#include "Button.hpp"

Button::Button(const base::collider& collider)
	:Actor(collider), m_onClickEvent(nullptr),m_shadowDraw(false), m_isCollided(false),m_isReleaseCalled(true),m_drawBox(false)
{
	m_shadow.setFillColor(sf::Color::White);
	m_shadow.setFillColor(sf::Color(255, 255, 255, 60));
}

void Button::setTexture(const sf::Texture& tex)
{
	m_sprite.setTexture(tex);
	m_shadow.setSize(sf::Vector2f(tex.getSize()));
}

void Button::onUpdate()
{
	if(!m_shadowDraw)
		m_shadow.setFillColor(sf::Color(255, 255, 255, 0));
	m_shadowDraw = false;

	if (m_isCollided)
	{
		m_drawBox = true;
	}
	else {
		m_drawBox = false;
		if (!m_isReleaseCalled)
		{
			if(m_onReleaseEvent != nullptr)
				m_onReleaseEvent();
			m_isReleaseCalled = true;
		}
	}
	m_isCollided = false;
}

void Button::onMouseCollision(bool isPressed)
{
	m_infoBox.refresh();
	m_isCollided = true;
	m_shadowDraw = true;
	m_isReleaseCalled = false;
	if (m_onClickEvent != nullptr)
	{
		m_onClickEvent(isPressed);
	}
	if (isPressed)
	{
		m_shadow.setFillColor(sf::Color(255, 255, 255, 140));
		return;
	}

	m_shadow.setFillColor(sf::Color(0, 0, 0, 50));
}

void Button::setPosition(const sf::Vector2f& pos)
{
	m_shadow.setPosition(pos);
	m_sprite.setPosition(pos);
	m_position = pos;
}

void Button::setScale(const sf::Vector2f& sca)
{
	m_shadow.setScale(sca);
	m_sprite.setScale(sca);
	m_scale = sca;
}

void Button::move(const sf::Vector2f& delta)
{
	m_shadow.move(delta);
	m_sprite.move(delta);
	m_position += delta;
}

Info& Button::getInfo()
{
	return m_infoBox;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
	target.draw(m_shadow, states);
	if (m_drawBox)
	{
		m_infoBox.draw(target,states);
	}
}

void Button::setCallbackOnMouseRelease(const std::function<void()>& func)
{
	m_onReleaseEvent = func;
}

void Button::setClickEvent(const std::function<void(bool)>& func)
{
	m_onClickEvent = func;
}
