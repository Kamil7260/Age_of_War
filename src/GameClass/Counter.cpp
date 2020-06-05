#include "Counter.hpp"

#include "../Core/Application.hpp"

Counter::Counter(int* target)
	:m_target(target), m_curTime(0.f), m_currentValue(0)
{
}

void Counter::setFont(const sf::Font& font)
{
	m_text.setFont(font);
}

void Counter::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_sprite, states);
	target.draw(m_text, states);
}

void Counter::setScale(const sf::Vector2f& sca)
{
	m_sprite->setScale(sca);
	m_text.setScale(sca);
	m_scale = sca;
}

void Counter::move(const sf::Vector2f& delta)
{
	m_sprite->move(delta);
	m_text.move(delta);
	m_position += delta;
}

void Counter::setPosition(const sf::Vector2f& pos)
{
	m_sprite->setPosition(pos);
	m_text.setPosition(pos + sf::Vector2f(20.f,-15.f));
	m_position = pos;
}

void Counter::setColor(const sf::Color& color)
{
	m_text.setFillColor(color);
}

void Counter::onUpdate()
{
	updateAnimator();
	if (!m_isRunning)
		play(m_currentClipName);

	m_curTime += core::Application::getInstance().getTime();
	if(*m_target != m_currentValue)
	if (m_curTime > 0.01f) {
		m_curTime = 0.f;
		if (m_currentValue < *m_target)
		{
			++m_currentValue;
		}
		else {
			--m_currentValue;
		}
		m_text.setString(std::to_string(m_currentValue));
	}
}

void Counter::addClip(base::Clip clip, const std::string& name)
{
	clip.setSprite(m_sprite);
	m_container.insert(std::make_pair(name, std::move(clip)));
	m_currentClipName = name;
}

void Counter::setCharacterSize(const unsigned int size)
{
	m_text.setCharacterSize(size);
}
