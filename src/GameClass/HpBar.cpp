#include "HpBar.hpp"


HpBar::HpBar(int* target, int x, int y)
	:m_x(x), m_y(y), m_target(target), m_current(*target), m_callBackOnDie(nullptr), m_call(false), m_maxHP(*target)
{
	m_frame = sf::VertexArray(sf::LinesStrip, 5);
	m_bar = sf::RectangleShape(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
	m_bar.setOrigin(sf::Vector2f(x / 2.f, static_cast<float>(y)));
	m_bar.setFillColor(sf::Color::Red);

	m_text.setString(std::to_string(m_current));
	m_text.setCharacterSize(12);
	m_text.setFillColor(sf::Color::Black);
}

void HpBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_bar, states);
	target.draw(&m_frame[0], 5, sf::LinesStrip);
	target.draw(m_text);
}

void HpBar::onUpdate()
{
	if (m_current != (*m_target))
	{
		m_current = *m_target;
		m_text.setString(std::to_string(m_current));

		if(m_current<=0 && !m_call)
			if (m_callBackOnDie != nullptr)
			{
				m_call = true;
				m_callBackOnDie();
			}
		m_bar.setScale(sf::Vector2f(1.f, static_cast<float>(m_current) / static_cast<float>(m_maxHP)));
	}
}

void HpBar::setPosition(const sf::Vector2f& pos)
{
	m_position = pos;

	m_frame[0].position = pos + sf::Vector2f(-m_x/ 2.f, -m_y/ 2.f);
	m_frame[1].position = pos + sf::Vector2f(m_x/ 2.f, -m_y/ 2.f);
	m_frame[2].position = pos + sf::Vector2f(m_x/ 2.f, m_y/ 2.f);
	m_frame[3].position = pos + sf::Vector2f(-m_x/ 2.f, m_y/ 2.f);
	m_frame[4].position = pos + sf::Vector2f(-m_x/ 2.f, -m_y/ 2.f);
	for (int k = 0; k < 5; ++k)
		m_frame[k].color = sf::Color::Black;
	m_bar.setPosition(sf::Vector2f(pos.x, pos.y + m_y / 2.f));
	m_text.setPosition(sf::Vector2f(m_position.x - m_x / 2.f, m_position.y - m_y / 1.7f));
}

void HpBar::move(const sf::Vector2f& delta)
{
	m_position += delta;
	m_frame[0].position = m_position + sf::Vector2f(-m_x / 2.f, -m_y / 2.f);
	m_frame[1].position = m_position + sf::Vector2f(m_x / 2.f, -m_y / 2.f);
	m_frame[2].position = m_position + sf::Vector2f(m_x / 2.f, m_y / 2.f);
	m_frame[3].position = m_position + sf::Vector2f(-m_x / 2.f, m_y / 2.f);
	m_frame[4].position = m_position + sf::Vector2f(-m_x / 2.f, -m_y / 2.f);

	m_bar.setPosition(sf::Vector2f(m_position.x, m_position.y + m_y / 2.f));
	m_text.setPosition(sf::Vector2f(m_position.x-m_x / 2, m_position.y -m_y / 1.7f));
}

void HpBar::setFont(const sf::Font& font)
{
	m_text.setFont(font);
}

void HpBar::setCharacterSize(const unsigned int size)
{
	m_text.setCharacterSize(size);
}

void HpBar::setCallbackOnDie(const std::function<void()>& func)
{
	m_callBackOnDie = func;
}
