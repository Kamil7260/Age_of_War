#pragma once

#include "../Base/Actor.hpp"

class Bullet : public base::Actor {
public:
	Bullet(int minDMG = 5, int maxDMG = 20, float speed = 1.f, const sf::Vector2f& dir = { 1.f,1.f });
	Bullet(const Bullet& source) = default;
	Bullet(Bullet&& source) = default;

	Bullet& operator=(const Bullet& source) = default;
	Bullet& operator=(Bullet&& source) = default;

	virtual ~Bullet() = default;

	virtual void setTexture(const sf::Texture& tex);
	virtual void setPosition(const sf::Vector2f& pos) override;
	virtual void setScale(const sf::Vector2f& sca) override;
	virtual void move(const sf::Vector2f& delta) override;
	virtual void onCollision(std::unique_ptr<Actor>& collision) override;
	virtual void onUpdate() override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	virtual void setRotation(const int rot) override {
		m_rotation = rot;
		m_sprite.setRotation(static_cast<float>(rot));
	}
protected:
	sf::Sprite m_sprite;
	sf::Vector2f m_direction;
	float m_speed;
	int m_maxDMG;
	int m_minDMG;
};