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
		_drawCannonPlaces = value;
	}

	virtual void spawnCannon(const int type);

	virtual void sellActive();
	virtual void sellCanceled();

	virtual void loadNextAge();

	virtual void income(const int value) {
		_coinCount += value;
	}

	virtual void baseUpgrade();

private:
	float _timer;
	std::array<sf::Sprite,3> _sprites;
	bool _enableSpawn;
	std::vector<std::pair<std::unique_ptr<base::Actor>, std::unique_ptr<base::Actor>>> _queue;
	std::string _currentAge;
	std::array<sf::Sprite,3> _cannonPlaces;
	bool _drawCannonPlaces;
	std::array<std::unique_ptr<Cannon>,3> _cannons;

	int _coinCount;
	bool _wantSell;

	int _sellClickCount;
	int _baseUpgrade;

	std::array<std::string, 5> _ages;
	int _expCount;
	int _upgradeCondition;
	int _cannonPlaceCost;
};

