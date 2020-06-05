#pragma once
#include <random>

#include "../Base/Stronghold.hpp"
#include "Melee.hpp"
#include "Cannon.hpp"


class Player : public base::Stronghold {
public:
	Player();
	Player(const Player&) = delete;
	Player(Player&&) = default;
	Player& operator=(const Player&) = delete;
	Player& operator=(Player&&) = default;

	virtual ~Player() = default;

	virtual void setPosition(const sf::Vector2f& pos) override;
	virtual void setScale(const sf::Vector2f& sca) override;

	virtual void move(const sf::Vector2f& delta) override;

	virtual void onUpdate() override;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void spawnObject(const unsigned int type);
	
	virtual void addToQueue(const unsigned int type);

	virtual void drawCannonPlaces(bool value) {
		m_drawCannonPlaces = value;
	}

	virtual void spawnCannon(const int type);

	virtual void sellActive();
	virtual void sellCanceled();

	virtual void loadNextAge();

	virtual void income(const int value) {
		m_coinCount += value;
	}

	virtual void baseUpgrade();

private:
	float m_timer;
	std::array<sf::Sprite,3> m_sprites;
	bool m_enableSpawn;
	std::vector<std::pair<std::unique_ptr<base::Actor>, std::unique_ptr<base::Actor>>> m_queue;
	std::string m_currentAge;
	std::array<sf::Sprite,3> m_cannonPlaces;
	bool m_drawCannonPlaces;
	std::array<std::unique_ptr<Cannon>,3> m_cannons;

	int m_coinCount;
	bool m_wantSell;

	int m_sellClickCount;
	int m_baseUpgrade;

	std::array<std::string, 5> m_ages;
	int m_expCount;
	int m_upgradeCondition;
	int m_cannonPlaceCost;
};

