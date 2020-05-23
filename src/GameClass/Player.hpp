#pragma once
#include <random>
#include <array>

#include "../Base/Stronghold.hpp"
#include "Melee.hpp"

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

	virtual bool loadFromJson(const std::string& type,const std::string& name, const unsigned int index);
	virtual void spawnObject(const unsigned int type);
	
private:
	float _timer;
	sf::Sprite _sprite;
	bool _enableSpawn;
	std::array<mobInfo, 3> _mobTemplate;
	std::vector<std::pair<std::unique_ptr<base::Actor>, std::unique_ptr<base::Actor>>> _queue;
};

