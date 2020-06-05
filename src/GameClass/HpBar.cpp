#include "HpBar.hpp"


HpBar::HpBar(int* target, int x, int y)
	:_x(x), _y(y), _target(target), _current(*target), _callBackOnDie(nullptr), _call(false), _maxHP(*target)
{
	_frame = sf::VertexArray(sf::LinesStrip, 5);
	_bar = sf::RectangleShape(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
	_bar.setOrigin(sf::Vector2f(x / 2.f, static_cast<float>(y)));
	_bar.setFillColor(sf::Color::Red);

	_text.setString(std::to_string(_current));
	_text.setCharacterSize(12);
	_text.setFillColor(sf::Color::Black);
}

void HpBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_bar, states);
	target.draw(&_frame[0], 5, sf::LinesStrip);
	target.draw(_text);
}

void HpBar::onUpdate()
{
	if (_current != (*_target))
	{
		_current = *_target;
		_text.setString(std::to_string(_current));

		if(_current<=0 && !_call)
			if (_callBackOnDie != nullptr)
			{
				_call = true;
				_callBackOnDie();
			}
		_bar.setScale(sf::Vector2f(1.f, static_cast<float>(_current) / static_cast<float>(_maxHP)));
	}
}

void HpBar::setPosition(const sf::Vector2f& pos)
{
	_position += pos;

	_frame[0].position = pos + sf::Vector2f(-_x/ 2.f, -_y/ 2.f);
	_frame[1].position = pos + sf::Vector2f(_x/ 2.f, -_y/ 2.f);
	_frame[2].position = pos + sf::Vector2f(_x/ 2.f, _y/ 2.f);
	_frame[3].position = pos + sf::Vector2f(-_x/ 2.f, _y/ 2.f);
	_frame[4].position = pos + sf::Vector2f(-_x/ 2.f, -_y/ 2.f);
	for (int k = 0; k < 5; ++k)
		_frame[k].color = sf::Color::Black;
	_bar.setPosition(sf::Vector2f(pos.x, pos.y + _y / 2.f));
	_text.setPosition(sf::Vector2f(_position.x - _x / 2.f, _position.y - _y / 1.7f));
}

void HpBar::move(const sf::Vector2f& delta)
{
	_position += delta;
	_frame[0].position = _position + sf::Vector2f(-_x / 2.f, -_y / 2.f);
	_frame[1].position = _position + sf::Vector2f(_x / 2.f, -_y / 2.f);
	_frame[2].position = _position + sf::Vector2f(_x / 2.f, _y / 2.f);
	_frame[3].position = _position + sf::Vector2f(-_x / 2.f, _y / 2.f);
	_frame[4].position = _position + sf::Vector2f(-_x / 2.f, -_y / 2.f);

	_bar.setPosition(sf::Vector2f(_position.x, _position.y + _y / 2.f));
	_text.setPosition(sf::Vector2f(_position.x-_x / 2, _position.y -_y / 1.7f));
}

void HpBar::setFont(const sf::Font& font)
{
	_text.setFont(font);
}

void HpBar::setCharacterSize(const unsigned int size)
{
	_text.setCharacterSize(size);
}

void HpBar::setCallbackOnDie(const std::function<void()>& func)
{
	_callBackOnDie = func;
}
