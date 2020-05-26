#include "Player.hpp"
#include "../Core/ResourceManager.hpp"
#include "../Core/Renderer.hpp"
#include "../Core/Application.hpp"
#include "Melee.hpp"
#include "Range.hpp"
#include "Button.hpp"
#include "QueueManager.hpp"
#include "CannonSpawner.hpp"


Player::Player()
	:_enableSpawn(true), _drawCannonPlaces(false)
{
	_cannon = nullptr;
	_tag = "Player";
	_currentAge = 'I';
	_timer = 0;
	_sprite.setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/base/1.png"));
	_sprite.setOrigin(275.f, 170.f);
	_myColider = { 111.f,111.f,111.f,111.f };

	_cannonPlace.setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/base/6.png"));
	_cannonPlace.setOrigin(sf::Vector2f(30.f, 30.f));
	auto manager = std::make_unique<QueueManager>();
	manager->setPosition(sf::Vector2f(300.f, 150.f));
	core::Renderer::getInstance().addObject(std::move(manager), base::object_type::gui);
	loadFromJson("Melee", "caveman", 0);
	loadFromJson("Range", "slingshoter", 1);
	loadFromJson("Melee", "dino", 2);
	loadCannonFromJson("C1");

	auto button = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
	button->setPosition(sf::Vector2f(300.f, 50.f));
	button->setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/I/1.png"));
	button->setClickEvent([&](bool isPressed)->void {
		if (isPressed && _enableSpawn)
		{
			addToQueue(0);
		}
		});

	core::Renderer::getInstance().addObject(std::move(button), base::object_type::gui);

	 button = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
	button->setPosition(sf::Vector2f(400.f, 50.f));
	button->setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/I/2.png"));
	button->setClickEvent([&](bool isPressed)->void {
		if (isPressed && _enableSpawn)
		{
			addToQueue(1);
		}
		});
	core::Renderer::getInstance().addObject(std::move(button), base::object_type::gui);
	
	 button = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
	button->setPosition(sf::Vector2f(500.f, 50.f));
	button->setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/I/3.png"));
	button->setClickEvent([&](bool isPressed)->void {
		if (isPressed && _enableSpawn)
		{
			addToQueue(2);
		}
		});
	core::Renderer::getInstance().addObject(std::move(button), base::object_type::gui);
	
	button = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
	button->setPosition(sf::Vector2f(700.f, 50.f));
	button->setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/I/4.png"));
	button->setClickEvent([&](bool isPressed)->void {
		if (_enableSpawn && isPressed)
		{
			_drawCannonPlaces = true;
			_enableSpawn = false;
			std::unique_ptr<CannonSpawner> ptr = std::make_unique<CannonSpawner>();
			ptr->setCallbackOnRelease([]()->void {
				auto& k = core::Renderer::getInstance().find("Player");
				if (k != nullptr)
				{
					auto ptr = static_cast<Player*>(k.get());
					ptr->spawnCannon(1);
				}
				});
			ptr->setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/I/4.png"));
			core::Renderer::getInstance().addObject(std::move(ptr), base::object_type::gui);
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
	_cannonPlace.setPosition(sf::Vector2f(pos.x +10 , pos.y - 90.f));
	if (_cannon != nullptr)
		_cannon->setPosition(pos);
}

void Player::setScale(const sf::Vector2f& sca)
{
	_scale = sca;
	_sprite.setScale(sca);
	_cannonPlace.scale(sca);
}

void Player::move(const sf::Vector2f& delta)
{
	_position += delta;
	_sprite.move(delta);
	//if (_cannon != nullptr)
	//	_cannon->move(delta);
	_cannonPlace.move(delta);
}

void Player::onUpdate()
{
	if (_timer >= 0.3f)
	{
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			_enableSpawn = true;
			_drawCannonPlaces = false;
		}
	}
	else _timer += core::Application::getInstance().getTime();

	if (_cannon != nullptr)
	{
		_cannon->onUpdate();
	}
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
	if (_cannon != nullptr)
	{
		target.draw(*_cannon, states);
	}
	else if (_drawCannonPlaces)
	{
		target.draw(_cannonPlace, states);
	}
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

bool Player::loadCannonFromJson(const std::string& name)
{
	auto& info = core::Application::getInstance().getCannonInfo();

	auto obj = info[_currentAge][name];
	if (obj == nullptr)
	{
		LOG_ERROR(name, " does not exist in json");
		return false;
	}

	auto animationSpeed = obj["animationSpeed"];
	if (animationSpeed == nullptr) {
		LOG_ERROR("Can not get animation speed for cannon : ", name);
		return false;
	}
	auto maxDMG = obj["maxDMG"];
	if (maxDMG == nullptr) {
		LOG_ERROR("Can not get max dmg for cannon : ", name);
		return false;
	}
	auto minDMG = obj["minDMG"];
	if (minDMG == nullptr) {
		LOG_ERROR("Can not get min dmg for cannon : ", name);
		return false;
	}
	auto range = obj["range"];
	if (range == nullptr) {
		LOG_ERROR("Can not get range for cannon : ", name);
		return false;
	}

	auto reload = obj["reloadTime"];
	if (range == nullptr) {
		LOG_ERROR("Can not get reloadTime for cannon : ", name);
		return false;
	}
	auto bulletSpeed = obj["bulletSpeed"];
	if (range == nullptr) {
		LOG_ERROR("Can not get bulletSpeed for cannon : ", name);
		return false;
	}

	_cannonInfo.animationSpeed = animationSpeed;
	_cannonInfo.maxDMG = maxDMG;
	_cannonInfo.minDMG = minDMG;
	_cannonInfo.range = range;
	_cannonInfo.reloadTime = reload;
	_cannonInfo.bulletSpeed = bulletSpeed;
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

void Player::addToQueue(const unsigned int type)
{
	_timer = 0.f;
	_enableSpawn = false;
	auto& manager = core::Renderer::getInstance().find("QueueManager");
	if (manager != nullptr)
	{
		auto queue = static_cast<QueueManager*>(manager.get());
		Ikon ikon;
		ikon.setTimePoint(_mobTemplate.at(type).spawnTime);
		std::string path = "Assets/gui/" + _currentAge + "/" + std::to_string(type + 1) + ".png";
		ikon.setTexture(*core::ResourceManager<sf::Texture>::getInstance().get(path.c_str()));
		queue->addIkon(ikon, [&, type]()->void {
			spawnObject(type);
			});
	}
	else {
		LOG_ERROR("Can not find QueueManager");
	}
}

void Player::spawnCannon(const int type)
{
	if (_cannon == nullptr)
	{
		auto pos = _cannonPlace.getPosition();
		auto mPos = core::Renderer::getInstance().getMousePosition();
		if (pos.x + 30.f > mPos.x && pos.x - 30 < mPos.x)
		{
			if (pos.y - 30 < mPos.y && pos.y + 30.f > mPos.y)
			{
				std::string name = _currentAge + "C" + std::to_string(type);
				_cannon = std::make_unique<Cannon>(_cannonInfo.maxDMG, _cannonInfo.minDMG, _cannonInfo.range,_cannonInfo.reloadTime,_cannonInfo.bulletSpeed);
				auto clip = core::Application::getInstance().getClip(name.c_str());
				_cannon->addClip(std::move(clip), name.c_str());
				name = "Assets/ammunition/" + _currentAge + "/C" + std::to_string(type) + ".png";
				_cannon->setBulletTexture(core::ResourceManager<sf::Texture>::getInstance().get(name.c_str()));
				_cannon->setAnimationSpeed(_cannonInfo.animationSpeed);
				_cannon->setPosition(_cannonPlace.getPosition());
			}
		}
	}
}