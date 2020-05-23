#include "Player.hpp"
#include "../Core/ResourceManager.hpp"
#include "../Core/Renderer.hpp"
#include "../Core/Application.hpp"
#include "Melee.hpp"
#include "Range.hpp"
#include "Button.hpp"

Player::Player()
	:_enableSpawn(true)
{
	_timer = 0;
	_sprite.setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/base/1.png"));
	_sprite.setOrigin(275.f, 170.f);
	_myColider = { 111.f,111.f,111.f,111.f };

	loadFromJson("Melee", "caveman", 0);
	loadFromJson("Range", "slingshoter", 1);
	loadFromJson("Melee", "dino", 2);

	auto button = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
	button->setPosition(sf::Vector2f(300.f, 50.f));
	button->setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/1.png"));
	button->setClickEvent([&](bool isPressed)->void {
		if (isPressed && _enableSpawn)
		{
			_enableSpawn = false;
			_timer = 0.f;
			spawnObject(0);
		}
		});

	core::Renderer::getInstance().addObject(std::move(button), base::object_type::gui);

	 button = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
	button->setPosition(sf::Vector2f(400.f, 50.f));
	button->setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/2.png"));
	button->setClickEvent([&](bool isPressed)->void {
		if (isPressed && _enableSpawn)
		{
			_enableSpawn = false;
			_timer = 0.f;
			spawnObject(1);
		}
		});
	core::Renderer::getInstance().addObject(std::move(button), base::object_type::gui);
	
	 button = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
	button->setPosition(sf::Vector2f(500.f, 50.f));
	button->setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/3.png"));
	button->setClickEvent([&](bool isPressed)->void {
		if (isPressed && _enableSpawn)
		{
			_enableSpawn = false;
			_timer = 0.f;
			spawnObject(2);
		}
		});
	core::Renderer::getInstance().addObject(std::move(button), base::object_type::gui);
	
}

Player::Player(Player&& source) noexcept
	:_timer(source._timer),_enableSpawn(source._enableSpawn)
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
	if (_timer >= 4.f)
		_enableSpawn = true;
	
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
}

bool Player::loadFromJson(const std::string& type, const std::string& name,const unsigned int index)
{
	auto& info = core::Application::getInstance().getMobInfo();

	auto obj = info[type][name];
	if (obj == nullptr)
	{
		LOG_ERROR(name, " does not exist in json");
		return false;
	}
	auto collider = obj["collider"];
	base::collider c;
	if (collider != nullptr)
	{
		c.up = collider.at(0);
		c.down = collider.at(1);
		c.left = collider.at(2);
		c.right = collider.at(3);
	}
	else {
		LOG_ERROR("Can not get collider ", name, " from json");
		return false;
	}
	auto hp = obj["hp"];
	if (hp == nullptr) {
		LOG_ERROR("Can not get hp ", name, " from json");
		return false;
	}
	auto minAttack = obj["minDMG"];
	auto maxAttack = obj["maxDMG"];
	auto speedAttack = obj["speedAttack"];
	if (maxAttack == nullptr || minAttack == nullptr || speedAttack == nullptr)
	{
		LOG_ERROR("Can not get attack ", name, " from json");
		return false;
	}
	auto animationSpeed = obj["animationSpeed"];
	if (animationSpeed == nullptr)
	{
		LOG_ERROR("Can not get animation speed from ", name, " json");
		return false;
	}
	auto speedMove = obj["speedMove"];
	if (speedMove == nullptr)
	{
		LOG_ERROR("Can not get speed move from ", name, " json");
		return false;
	}

	auto scale = obj["scale"];
	if (scale == nullptr)
	{
		LOG_ERROR("Can not get scale from ", name, " json");
		return false;
	}

	auto spawnTime = obj["spawnTime"];
	if (spawnTime == nullptr)
	{
		LOG_ERROR("Can not get spawn time from ", name, " json");
		return false;
	}
	if (type == "Range") {
		auto range = obj["range"];
		if (range == nullptr)
		{
			LOG_ERROR("Can not get range from ", name, " json");
			return false;
		}
		auto& t = _mobTemplate.at(index);
		t.animationSpeed = animationSpeed;
		t.collider = c;
		t.hp = hp;
		t.maxDMG = maxAttack;
		t.minDMG = minAttack;
		t.range = range;
		t.scale.x = scale.at(0);
		t.scale.y = scale.at(1);
		t.speedAttack = speedAttack;
		t.speedMove = speedMove;
		t.name = name;
		t.spawnTime = spawnTime;
	}
	else {
		auto& t = _mobTemplate.at(index);
		t.animationSpeed = animationSpeed;
		t.collider = c;
		t.hp = hp;
		t.maxDMG = maxAttack;
		t.minDMG = minAttack;
		t.scale.x = scale.at(0);
		t.scale.y = scale.at(1);
		t.speedAttack = speedAttack;
		t.speedMove = speedMove;
		t.name = name;
		t.spawnTime = spawnTime;
	}
	return true;
}

void Player::spawnObject(const unsigned int type)
{
	if (type == 1)
	{
		auto& k = _mobTemplate.at(type);
		std::unique_ptr<Range> man = std::make_unique<Range>(k.collider, k.hp, k.minDMG, k.maxDMG, k.range, k.speedAttack, k.speedMove);
		man->setScale(sf::Vector2f(k.scale.x, k.scale.y));
		man->setAnimatorName(k.name.c_str());
		man->setAnimationSpeed(k.animationSpeed);
		man->setPosition(sf::Vector2f(_position.x + 100, _position.y + 75));
		core::Renderer::getInstance().addObject(std::move(man), base::object_type::actor);
		return;
	}
	auto& k = _mobTemplate.at(type);
	std::unique_ptr<Melee> man = std::make_unique<Melee>(k.collider, k.hp, k.minDMG, k.maxDMG, k.speedAttack, k.speedMove);
	man->setScale(sf::Vector2f(k.scale.x, k.scale.y));
	man->setAnimatorName(k.name.c_str());
	man->setAnimationSpeed(k.animationSpeed);
	man->setPosition(sf::Vector2f(_position.x + 100, _position.y + 75));
	core::Renderer::getInstance().addObject(std::move(man), base::object_type::actor);
}
