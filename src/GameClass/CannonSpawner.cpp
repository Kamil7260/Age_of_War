#include "CannonSpawner.hpp"
#include "../Core/Renderer.hpp"
#include "Player.hpp"

CannonSpawner::CannonSpawner()
	:m_spawnCallback(nullptr)
{

}

void CannonSpawner::setTexture(const sf::Texture& tex)
{
	m_sprite.setTexture(tex);
	auto v = tex.getSize();
	v.x /= 2;
	v.y /= 2;
	m_sprite.setOrigin(static_cast<sf::Vector2f>(v));
}

void CannonSpawner::move(const sf::Vector2f& delta)
{
	m_position += delta;
	m_sprite.move(delta);
}

void CannonSpawner::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

void CannonSpawner::onUpdate()
{
	setPosition(sf::Vector2f(core::Renderer::getInstance().getMousePosition()));
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		m_isActive = false;
		if (m_spawnCallback != nullptr)
			m_spawnCallback();
		core::Renderer::getInstance().clearNoActive();
	}
}

void CannonSpawner::setPosition(const sf::Vector2f& pos)
{
	m_sprite.setPosition(pos);
	m_position = pos;
}

void CannonSpawner::setScale(const sf::Vector2f& sca)
{
	m_sprite.setScale(sca);
	m_scale = sca;
}

void CannonSpawner::setCallbackOnRelease(const std::function<void()>& callme)
{
	m_spawnCallback = callme;
}
