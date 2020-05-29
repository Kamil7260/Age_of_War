#pragma once

#include "../Base/ActorAnimator.hpp"

class CoinCounter : public base::ActorAnimator
{
public:
	CoinCounter(int* target);
	CoinCounter(const CoinCounter&) = default;
	CoinCounter(CoinCounter&&) = default;

	CoinCounter& operator=(const CoinCounter&) = default;
	CoinCounter& operator=(CoinCounter&&) = default;
	~CoinCounter() = default;

	void setFont(const sf::Font& font);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setScale(const sf::Vector2f& sca) override;

	void move(const sf::Vector2f& delta) override;

	void setPosition(const sf::Vector2f& pos) override;

	void setColor(const sf::Color& color);

	void onUpdate() override;

	void setCharacterSize(const unsigned int size);
protected:
	sf::Text _text;
	int* _target;

	float _curTime;
	int _currentCoin;
};