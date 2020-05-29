#pragma once
#include <string>

#include "../Base/Actor.hpp"

class TextDisplay : public base::Actor {
public:
	TextDisplay(float maxTime = 2.5f, float speed = 20.f);
	TextDisplay(const TextDisplay&) = default;
	TextDisplay(TextDisplay&&) = default;

	TextDisplay& operator=(const TextDisplay&) = default;
	TextDisplay& operator=(TextDisplay&&) = default;

	virtual  ~TextDisplay() = default;

	virtual void move(const sf::Vector2f& delta) override;
	virtual void onUpdate() override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void setPosition(const sf::Vector2f& pos) override;
	virtual void setScale(const sf::Vector2f& sca) override;

	virtual void setColor(const sf::Color& color);

	virtual void setCharacterSize(const unsigned int size);

	virtual void setIncome(const int income);

	virtual void setFont(const sf::Font& font);

	virtual void setTexture(const sf::Texture& tex);
protected:
	sf::Sprite _icon;
	sf::Text _text;
	float _curTime;
	float _maxTime;
	float _speed;
	int _income;
	bool _action;
};	