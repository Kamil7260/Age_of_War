#pragma once
#include <SFML/Graphics.hpp>

#include "../Base/Actor.hpp"

class BackGround : public base::Actor {
public:

	BackGround();

	BackGround(const BackGround&) = default;
	BackGround(BackGround&&) = default;
	
	BackGround& operator=(const BackGround&) = default;
	BackGround& operator=(BackGround&&) = default;

	virtual ~BackGround() = default;

	virtual void setPosition(const sf::Vector2f& pos) override;
	virtual void setScale(const sf::Vector2f& sca) override;

	virtual void move(const sf::Vector2f& delta) override;

	virtual void onUpdate() override;

	virtual void setTexture(const sf::Texture& tex) { _sprite.setTexture(tex); }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Sprite _sprite;


};