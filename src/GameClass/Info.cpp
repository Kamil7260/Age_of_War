#include "Info.hpp"

void Info::setPosition(const sf::Vector2f& pos)
{
	m_position = pos;
	m_text.setPosition(sf::Vector2f(pos.x+15.f,pos.y+15));
	m_sprite.setPosition(pos);
}

void Info::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
	target.draw(m_text, states);
}

void Info::onUpdate()
{

}

void Info::setString(const std::string& info)
{
	m_text.setString(info);
}

void Info::setScale(const sf::Vector2f& sca)
{
	m_text.setScale(sca);
	m_sprite.setScale(sca);
	m_scale = sca;
}

void Info::setFont(const sf::Font& font)
{
	m_text.setFont(font);
}

void Info::setColor(const sf::Color& color)
{
	m_text.setFillColor(color);
}

void Info::setTexture(const sf::Texture& tex)
{
	m_sprite.setTexture(tex);
}

void Info::setCharacterSize(const unsigned int size)
{
	m_text.setCharacterSize(size);
}

void Info::refresh()
{
	std::string text;
	for (auto& k : m_container)
	{
		text += k.first + k.second->getString() + "\n";
	}
	m_text.setString(text);
}

void Info::clear()
{
	m_container.clear();
}
