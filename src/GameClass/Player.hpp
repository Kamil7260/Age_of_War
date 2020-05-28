#pragma once
#include <random>
#include <array>

#include "../Base/Stronghold.hpp"
#include "Melee.hpp"
#include "Cannon.hpp"


struct mobInfo {
	std::string name;
	base::collider collider;
	int hp;
	int maxDMG;
	int minDMG;
	float speedAttack;
	float animationSpeed;
	float speedMove;
	sf::Vector2f scale;
	int range;
	float spawnTime;
	float reloadTime;
	float bulletSpeed;
};


class Player : public base::Stronghold {
public:
	Player();
	Player(const Player&) = delete;
	Player(Player&&) noexcept;
	Player& operator=(const Player&) = delete;
	Player& operator=(Player&&) noexcept;

	virtual void setPosition(const sf::Vector2f& pos) override;
	virtual void setScale(const sf::Vector2f& sca) override;

	virtual void move(const sf::Vector2f& delta) override;

	virtual void onUpdate() override;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual bool loadFromJson(const unsigned int index);
	virtual bool loadCannonFromJson(const std::string& name);
	virtual void spawnObject(const unsigned int type);
	
	virtual void addToQueue(const unsigned int type);

	virtual void drawCannonPlaces(bool value) {
		_drawCannonPlaces = value;
	}

	virtual void spawnCannon(const int type);

	virtual void loadNextAge(const std::string& index);

private:
	float _timer;
	sf::Sprite _sprite;
	bool _enableSpawn;
	std::array<mobInfo, 3> _mobTemplate;
	mobInfo _cannonInfo;
	std::vector<std::pair<std::unique_ptr<base::Actor>, std::unique_ptr<base::Actor>>> _queue;
	std::string _currentAge;
	sf::Sprite _cannonPlace;
	bool _drawCannonPlaces;
	std::unique_ptr<Cannon> _cannon;
};

