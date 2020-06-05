#pragma once
#include <random>

#include "../Base/Stronghold.hpp"

#include "../Base/JsonFunctions.hpp"

class Enemy : public base::Stronghold {
public:
	Enemy();
	Enemy(const Enemy&) = delete;
	Enemy(Enemy&&) = default;
	Enemy& operator=(const Enemy&) = delete;
	Enemy& operator=(Enemy&&) = default;

	virtual ~Enemy() = default;

	virtual void setPosition(const sf::Vector2f& pos) override;
	virtual void setScale(const sf::Vector2f& sca) override;

	virtual void move(const sf::Vector2f& delta) override;

	virtual void onUpdate() override;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	float m_timer;
	sf::Sprite m_sprite;
	std::array<base::unitInfo, 3> m_mobTemplate;
	std::string m_currentAge;
	
};