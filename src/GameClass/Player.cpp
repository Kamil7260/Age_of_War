#include "Player.hpp"
#include "../Core/ResourceManager.hpp"
#include "../Core/Renderer.hpp"
#include "../Core/Application.hpp"
#include "Melee.hpp"

Player::Player()
{
	_timer = 0;
	_sprite.setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/base/1.png"));
	_sprite.setOrigin(275.f, 170.f);
	_myColider = { 111.f,111.f,111.f,111.f };
}

Player::Player(Player&& source) noexcept
	:_timer(source._timer)
{
}

Player& Player::operator=(Player&& source) noexcept
{
	_timer = source._timer;
	return *this;
}

void Player::setPosition(const sf::Vector2f& pos)
{
	_position = pos;
	_sprite.setPosition(pos);
}

void Player::setScale(const sf::Vector2f& sca)
{
	_scale = sca;
	_sprite.setScale(sca);
}

void Player::move(const sf::Vector2f& delta)
{
	_position += delta;
	_sprite.move(delta);
}

void Player::onUpdate()
{
	_timer += core::Application::getInstance().getTime();

	if (_timer >= 5)
	{
		std::unique_ptr<Melee> man = std::make_unique<Melee>();
		man->setAnimatorName("caveman");
		man->setPosition(sf::Vector2f(130, 850));
		core::Renderer::getInstance().addObject(std::move(man), base::object_type::actor);
		_timer = 0;
	}
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
}
