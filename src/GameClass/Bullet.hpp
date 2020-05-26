#pragma once

#include "../Base/Actor.hpp"

class Bullet : public base::Actor {
public:
	Bullet(int minDMG = 5, int maxDMG = 20, float speed = 1.f, const sf::Vector2f& dir = { 1.f,1.f });
	Bullet(const Bullet& source);
	Bullet(Bullet&& source) noexcept;

	Bullet& operator=(const Bullet& source);
	Bullet& operator=(Bullet&& source) noexcept;

	virtual void setTexture(const sf::Texture& tex);
	virtual void setPosition(const sf::Vector2f& pos) override;
	virtual void setScale(const sf::Vector2f& sca) override;
	virtual void move(const sf::Vector2f& delta) override;
	virtual void onCollision(std::unique_ptr<Actor>& collision) override;
	virtual void onUpdate() override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
	sf::Sprite _sprite;
	sf::Vector2f _direction;
	float _speed;
	int _maxDMG;
	int _minDMG;
};