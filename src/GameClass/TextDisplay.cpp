#include <cmath>

#include "TextDisplay.hpp"
#include "../Core/Renderer.hpp"
#include "../Core/Application.hpp"

#include "Player.hpp"

TextDisplay::TextDisplay(float maxTime, float speed)
	:_maxTime(maxTime), _curTime(0.f),_speed(speed), _income(0), _action(false)
{
}

void TextDisplay::move(const sf::Vector2f& delta)
{
	_text.move(sf::Vector2f(delta));
	_icon.move(sf::Vector2f(delta));
	_position += delta;
}

void TextDisplay::onUpdate()
{
	auto delta = core::Application::getInstance().getTime();
	_curTime += delta;

	auto deltaP = _speed * delta * (_maxTime - _curTime) / _maxTime;
	move(sf::Vector2f(0.f, -deltaP));
	if (_curTime > 1.f)
	{
		if (!_action) {
			auto& p = core::Renderer::getInstance().find("Player");
			auto s_ptr = static_cast<Player*>(p.get());
			s_ptr->income(_income);
			_action = true;
		}
	}
	if (_curTime > _maxTime)
	{
		remove();
		core::Renderer::getInstance().clearNoActive();
	}
}

void TextDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_text, states);
	target.draw(_icon, states);
}

void TextDisplay::setPosition(const sf::Vector2f& pos)
{
	_position = pos;
	_text.setPosition(pos);
	_icon.setPosition(sf::Vector2f(pos.x-30.f,pos.y));
}

void TextDisplay::setScale(const sf::Vector2f& sca)
{
	_scale = sca;
	_text.setScale(sca);
	_icon.setScale(sca);
}

void TextDisplay::setColor(const sf::Color& color)
{
	_text.setFillColor(color);
}

void TextDisplay::setCharacterSize(const unsigned int size)
{
	_text.setCharacterSize(size);
}

void TextDisplay::setIncome(const int income)
{
	_income = income;
	_text.setString(std::to_string(_income));
}

void TextDisplay::setFont(const sf::Font& font)
{
	_text.setFont(font);
}

void TextDisplay::setTexture(const sf::Texture& tex)
{
	_icon.setTexture(tex);
}
