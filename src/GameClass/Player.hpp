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

	virtual void loadNextAge();

	virtual void income(const int value) {
		_coinCount += value;
	}

private:
	float _timer;
	sf::Sprite _sprite;
	bool _enableSpawn;
	std::vector<std::pair<std::unique_ptr<base::Actor>, std::unique_ptr<base::Actor>>> _queue;
	std::string _currentAge;
	sf::Sprite _cannonPlace;
	bool _drawCannonPlaces;
	std::unique_ptr<Cannon> _cannon;

	int _coinCount;
};

