#include "CannonSpawner.hpp"
#include "../Core/Renderer.hpp"
#include "Player.hpp"

CannonSpawner::CannonSpawner()
	:_spawnCallback(nullptr)
{

}

void CannonSpawner::setTexture(const sf::Texture& tex)
{
	_sprite.setTexture(tex);
	auto v = tex.getSize();
	v.x /= 2;
	v.y /= 2;
	_sprite.setOrigin(static_cast<sf::Vector2f>(v));
}

void CannonSpawner::move(const sf::Vector2f& delta)
{
	_position += delta;
	_sprite.move(delta);
}

void CannonSpawner::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
}

void CannonSpawner::onUpdate()
{
	setPosition(sf::Vector2f(core::Renderer::getInstance().getMousePosition()));
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		_isActive = false;
		if (_spawnCallback != nullptr)
			_spawnCallback();
		core::Renderer::getInstance().clearNoActive();
	}
}

void CannonSpawner::setPosition(const sf::Vector2f& pos)
{
	_sprite.setPosition(pos);
	_position = pos;
}

void CannonSpawner::setScale(const sf::Vector2f& sca)
{
	_sprite.setScale(sca);
	_scale = sca;
}

void CannonSpawner::setCallbackOnRelease(const std::function<void()>& callme)
{
	_spawnCallback = callme;
}
