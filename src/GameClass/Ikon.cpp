#include "Ikon.hpp"

#include "../Core/Application.hpp"

Ikon::Ikon(float time)
	:m_maxTime(time), m_timer(0.f)
{
	m_shadow.setFillColor(sf::Color::Black);
	m_shadow.setFillColor(sf::Color(0, 0, 0, 120));
}

void Ikon::onUpdate()
{
	float delta = core::Application::getInstance().getTime();
	m_timer += delta;

	if(m_timer>m_maxTime)
		if (m_onEnd != nullptr)
		{
			m_onEnd();
		}
	m_shadow.setSize(sf::Vector2f(m_shadow.getSize().x, m_shadowSize.y * (m_maxTime-m_timer)/m_maxTime));
}

void Ikon::setPosition(const sf::Vector2f& pos)
{
	m_sprite.setPosition(pos);
	m_shadow.setPosition(pos);
	m_position = pos;
}

void Ikon::setScale(const sf::Vector2f& sca)
{
	m_sprite.setScale(sca);
	m_shadow.setScale(sca);
	m_scale = sca;
}

void Ikon::move(const sf::Vector2f& delta)
{
	m_sprite.move(delta);
	m_shadow.move(delta);
	m_position += delta;
}

void Ikon::setTexture(const sf::Texture& tex)
{
	m_sprite.setTexture(tex);
	m_shadowSize = sf::Vector2f(tex.getSize());
	m_shadow.setSize(m_shadowSize);
}

void Ikon::setTimePoint(const float points)
{
	m_maxTime = points;
}

void Ikon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
	target.draw(m_shadow, states);
}

void Ikon::setCallBackOnEnd(const std::function<void()>& onEnd)
{
	m_onEnd = onEnd;
}
