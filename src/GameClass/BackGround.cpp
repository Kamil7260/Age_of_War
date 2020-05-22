#include "Background.hpp"

#include "../Core/ResourceManager.hpp"

BackGround::BackGround()
{
}

void BackGround::setPosition(const sf::Vector2f& pos)
{
	Actor::setPosition(pos);
	_sprite.setPosition(pos);
}

void BackGround::setScale(const sf::Vector2f& sca)
{
	Actor::setScale(sca);
	_sprite.setScale(sca);
}

void BackGround::move(const sf::Vector2f& delta)
{
	Actor::move(delta);
	_sprite.move(delta);
}

void BackGround::onUpdate()
{
}

void BackGround::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
}
