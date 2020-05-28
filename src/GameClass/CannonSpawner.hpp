#pragma once
#include "../Base/Actor.hpp"
#include <functional>
class CannonSpawner : public base::Actor {
public:
	CannonSpawner();
	CannonSpawner(const CannonSpawner& source) = default;
	CannonSpawner(CannonSpawner&& source) = default;

	CannonSpawner& operator=(const CannonSpawner& source) = default;
	CannonSpawner& operator=(CannonSpawner&& source) = default;

	virtual ~CannonSpawner() = default;

	virtual void setTexture(const sf::Texture& tex);
	virtual void move(const sf::Vector2f& delta) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void onUpdate() override;
	virtual void setPosition(const sf::Vector2f& pos) override;
	virtual void setScale(const sf::Vector2f& sca) override;

	virtual void setCallbackOnRelease(const std::function<void()>& callme);
protected:
	sf::Sprite _sprite;
	std::function<void()> _spawnCallback;
};