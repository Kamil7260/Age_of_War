#include <cmath>

#include "TextDisplay.hpp"
#include "../Core/Renderer.hpp"
#include "../Core/Application.hpp"

#include "Player.hpp"

TextDisplay::TextDisplay(float maxTime, float speed)
	:m_maxTime(maxTime), m_curTime(0.f),m_speed(speed), m_income(0), m_action(false)
{
}

void TextDisplay::move(const sf::Vector2f& delta)
{
	m_text.move(sf::Vector2f(delta));
	m_icon.move(sf::Vector2f(delta));
	m_position += delta;
}

void TextDisplay::onUpdate()
{
	auto delta = core::Application::getInstance().getTime();
	m_curTime += delta;

	auto deltaP = m_speed * delta * (m_maxTime - m_curTime) / m_maxTime;
	move(sf::Vector2f(0.f, -deltaP));
	if (m_curTime > 1.f)
	{
		if (!m_action) {
			auto& p = core::Renderer::getInstance().find("Player");
			auto s_ptr = static_cast<Player*>(p.get());
			s_ptr->income(m_income);
			m_action = true;
		}
	}
	if (m_curTime > m_maxTime)
	{
		remove();
		core::Renderer::getInstance().clearNoActive();
	}
}

void TextDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_text, states);
	target.draw(m_icon, states);
}

void TextDisplay::setPosition(const sf::Vector2f& pos)
{
	m_position = pos;
	m_text.setPosition(pos);
	m_icon.setPosition(sf::Vector2f(pos.x-30.f,pos.y));
}

void TextDisplay::setScale(const sf::Vector2f& sca)
{
	m_scale = sca;
	m_text.setScale(sca);
	m_icon.setScale(sca);
}

void TextDisplay::setColor(const sf::Color& color)
{
	m_text.setFillColor(color);
}

void TextDisplay::setCharacterSize(const unsigned int size)
{
	m_text.setCharacterSize(size);
}

void TextDisplay::setIncome(const int income)
{
	m_income = income;
	m_text.setString(std::to_string(m_income));
}

void TextDisplay::setFont(const sf::Font& font)
{
	m_text.setFont(font);
}

void TextDisplay::setTexture(const sf::Texture& tex)
{
	m_icon.setTexture(tex);
}
