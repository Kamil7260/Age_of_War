#pragma once
#include "../Base/Stronghold.hpp"

class Player : public base::Stronghold {
public:
	Player();
	Player(const Player&) = delete;
	Player(Player&&);
	Player& operator=(const Player&) = delete;
	Player& operator=(Player&&);

	virtual void setPosition(const sf::Vector2f& pos) override;
	virtual void setScale(const sf::Vector2f& sca) override;

	virtual void move(const sf::Vector2f& delta) override;

	virtual void onUpdate() override;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	float _timer;
	sf::Sprite _sprite;
};