#include "Info.hpp"

void Info::setPosition(const sf::Vector2f& pos)
{
	_position = pos;
	_text.setPosition(sf::Vector2f(pos.x+15.f,pos.y+15));
	_sprite.setPosition(pos);
}

void Info::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
	target.draw(_text, states);
}

void Info::onUpdate()
{

}

void Info::setString(const std::string& info)
{
	_text.setString(info);
}

void Info::setScale(const sf::Vector2f& sca)
{
	_text.setScale(sca);
	_sprite.setScale(sca);
	_scale = sca;
}

void Info::setFont(const sf::Font& font)
{
	_text.setFont(font);
}

void Info::setColor(const sf::Color& color)
{
	_text.setFillColor(color);
}

void Info::setTexture(const sf::Texture& tex)
{
	_sprite.setTexture(tex);
}

void Info::setCharacterSize(const unsigned int size)
{
	_text.setCharacterSize(size);
}

void Info::refresh()
{
	std::string text;
	for (auto& k : _container)
	{
		text += k.first + k.second->getString() + "\n";
	}
	_text.setString(text);
}

void Info::clear()
{
	_container.clear();
}
