#pragma once
#include <random>

#include "../Base/Stronghold.hpp"

class Enemy : public base::Stronghold {
public:
	Enemy();
	Enemy(const Enemy&) = delete;
	Enemy(Enemy&&) noexcept;
	Enemy& operator=(const Enemy&) = delete;
	Enemy& operator=(Enemy&&) noexcept;

	virtual void setPosition(const sf::Vector2f& pos) override;
	virtual void setScale(const sf::Vector2f& sca) override;

	virtual void move(const sf::Vector2f& delta) override;

	virtual void onUpdate() override;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	float _timer;
	sf::Sprite _sprite;
};