#pragma once
#include <random>

#include "../Base/Stronghold.hpp"

#include "../Base/JsonFunctions.hpp"

#include "../Logger/Logger.hpp"

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

	virtual void loadNewAge();

	virtual void addIncome(const unsigned int income)
	{
		m_exp += income;
	}
private:
	float m_timer;
	sf::Sprite m_sprite;
	std::array<base::unitInfo, 3> m_mobTemplate;
	std::array<std::string, 5> m_ages;
	int m_currentAge;

	std::vector<std::pair<std::unique_ptr<base::Actor>,float>> m_queue;
	int m_groupOverAll;
	float m_breakTime;
	int m_exp;
	int m_requiredExp;
};