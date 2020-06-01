#include "Player.hpp"
#include "../Core/ResourceManager.hpp"
#include "../Core/Renderer.hpp"
#include "../Core/Application.hpp"
#include "Melee.hpp"
#include "Range.hpp"
#include "Button.hpp"
#include "QueueManager.hpp"
#include "CannonSpawner.hpp"
#include "CoinCounter.hpp"

Player::Player()
	:_enableSpawn(true), _drawCannonPlaces(false), _coinCount(100), _wantSell(false), _sellClickCount(0)
{
	_cannon = nullptr;
	_tag = "Player";
	_currentAge = "I";
	_timer = 0;
	_sprite.setOrigin(275.f, 170.f);
	_myColider = { 111.f,111.f,111.f,111.f };


	_cannonPlace.setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/base/6.png"));
	_cannonPlace.setOrigin(sf::Vector2f(30.f, 30.f));

	auto manager = std::make_unique<QueueManager>();
	manager->setPosition(sf::Vector2f(300.f, 150.f));
	core::Renderer::getInstance().addObject(std::move(manager), base::object_type::gui);


	auto button = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
	button->setTag("Button");
	core::Renderer::getInstance().forceAddObject(std::move(button), base::object_type::gui);

	button = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
	button->setTag("Button");
	core::Renderer::getInstance().forceAddObject(std::move(button), base::object_type::gui);

	button = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
	button->setTag("Button");
	core::Renderer::getInstance().forceAddObject(std::move(button), base::object_type::gui);

	button = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
	button->setTag("Button");
	core::Renderer::getInstance().forceAddObject(std::move(button), base::object_type::gui);

	button = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
	button->setTag("Button");
	core::Renderer::getInstance().forceAddObject(std::move(button), base::object_type::gui);

	button = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
	button->setTag("Button");
	core::Renderer::getInstance().forceAddObject(std::move(button), base::object_type::gui);

	button = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
	button->setTag("SellButton");
	button->setClickEvent([&](bool isPressed)->void {
		if (isPressed) {
			this->sellActive();
		}
	});
	button->setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/2.png"));

	button->setPosition(sf::Vector2f(1100, 50));
	core::Renderer::getInstance().addObject(std::move(button), base::object_type::gui);
	loadNextAge();

	std::unique_ptr<CoinCounter> coin = std::make_unique<CoinCounter>(&_coinCount);
	coin->setPosition(sf::Vector2f(1600, 40));
	coin->setFont(*core::ResourceManager<sf::Font>::getInstance().get("Assets/fonts/3.ttf"));
	coin->setCharacterSize(20);
	coin->setColor(sf::Color::Yellow);
	core::Renderer::getInstance().addObject(std::move(coin), base::object_type::gui);
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
	if (_cannon != nullptr)
		_cannon->move(delta);
	_cannonPlace.move(delta);
}

void Player::onUpdate()
{
	if (_timer >= 0.3f)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
		{
			_currentAge = "V";
			loadNextAge();
		}
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			_enableSpawn = true;
			if(!_wantSell)
				_drawCannonPlaces = false;
		}
	}
	else _timer += core::Application::getInstance().getTime();

	if (_cannon != nullptr)
	{
		_cannon->onUpdate();
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (_wantSell)
			{
				++_sellClickCount;
				if (_sellClickCount > 1)
				{
					auto pos = _cannon->getPosition();
					auto mPos = core::Renderer::getInstance().getMousePosition();
					if (pos.x + 30.f > mPos.x && pos.x - 30 < mPos.x)
					{
						if (pos.y - 30 < mPos.y && pos.y + 30.f > mPos.y)
						{
							_coinCount += _cannon->getSellPrice();
							_cannon = nullptr;
						}
					}
					_sellClickCount = 0;
					_wantSell = false;
					_drawCannonPlaces = false;
					sellCanceled();
				}
			}
			else {
				_sellClickCount = 0;
			}
		}
	}
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
	if (_cannon != nullptr)
	{
		target.draw(*_cannon, states);
		if(_wantSell)
			target.draw(_cannonPlace, states);
	}
	else if (_drawCannonPlaces)
	{
		target.draw(_cannonPlace, states);
	}
}


void Player::spawnObject(const unsigned int type)
{
	if (type == 1)
	{
		auto& k = _mobTemplate.at(type);
		std::unique_ptr<Range> man = std::make_unique<Range>(k.collider, k.hp, k.minDMG, k.maxDMG, k.range, k.speedAttack, k.speedMove,k.income);
		man->setScale(sf::Vector2f(k.scale.x, k.scale.y));
		man->setAnimatorName(k.name);
		man->setAnimationSpeed(k.animationSpeed);
		man->setPosition(sf::Vector2f(_position.x + 100, _position.y + 75));
		core::Renderer::getInstance().addObject(std::move(man), base::object_type::actor);
		return;
	}
	auto& k = _mobTemplate.at(type);
	std::unique_ptr<Melee> man = std::make_unique<Melee>(k.collider, k.hp, k.minDMG, k.maxDMG, k.speedAttack, k.speedMove, k.income);
	man->setScale(sf::Vector2f(k.scale.x, k.scale.y));
	man->setAnimatorName(k.name);
	man->setAnimationSpeed(k.animationSpeed);
	man->setPosition(sf::Vector2f(_position.x + 100, _position.y + 75));
	core::Renderer::getInstance().addObject(std::move(man), base::object_type::actor);
}

void Player::addToQueue(const unsigned int type)
{
	if (_mobTemplate.at(0).price > _coinCount)
		return;

	_coinCount -= _mobTemplate.at(0).price;

	_timer = 0.f;
	_enableSpawn = false;
	auto& manager = core::Renderer::getInstance().find("QueueManager");
	if (manager != nullptr)
	{
		auto queue = static_cast<QueueManager*>(manager.get());
		Ikon ikon;
		ikon.setTimePoint(_mobTemplate.at(type).spawnTime);
		std::string path = "Assets/gui/" + _currentAge + "/" + std::to_string(type + 1) + ".png";
		ikon.setTexture(*core::ResourceManager<sf::Texture>::getInstance().get(path));
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
		if (_cannonTemplate.at(type).price > _coinCount)
			return;

		auto pos = _cannonPlace.getPosition();
		auto mPos = core::Renderer::getInstance().getMousePosition();
		if (pos.x + 30.f > mPos.x && pos.x - 30 < mPos.x)
		{
			if (pos.y - 30 < mPos.y && pos.y + 30.f > mPos.y)
			{
				auto& k = _cannonTemplate.at(type);
				std::string name = _currentAge + "C" + std::to_string(type);
				_cannon = std::make_unique<Cannon>(k.maxDMG, k.minDMG, k.range,k.reloadTime,k.bulletSpeed,k.bulletPosition,k.fireSpeed,k.price/2);
				if (k.long_range)
					_cannon->longRange();
				auto clip = core::Application::getInstance().getClip(name);
				_cannon->addClip(std::move(clip), name);
				name = "Assets/ammunition/" + _currentAge + "/C" + std::to_string(type) + ".png";
				_cannon->setBulletTexture(core::ResourceManager<sf::Texture>::getInstance().get(name));
				_cannon->setAnimationSpeed(k.animationSpeed);
				_cannon->setPosition(_cannonPlace.getPosition());
				_coinCount -= k.price;
			}
		}
	}
}

void Player::sellActive()
{
	if (_cannon != nullptr)
	{
		_drawCannonPlaces = true;
		_wantSell = true;
		core::Application::getInstance().setCursor(1);
	}
	_sellClickCount = 0;
}

void Player::sellCanceled()
{
	_wantSell = false;
	core::Application::getInstance().setCursor(0);
}

void Player::loadNextAge()
{

	auto& infoBlock = core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/5.png");
	auto& font = core::ResourceManager<sf::Font>::getInstance().get("Assets/fonts/3.ttf");

	_sprite.setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/base/" + _currentAge + ".png"));
	_mobTemplate.at(0) = base::loadUnitFromJson(0, _currentAge);
	_mobTemplate.at(1) = base::loadUnitFromJson(1, _currentAge);
	_mobTemplate.at(2) = base::loadUnitFromJson(2, _currentAge);
	_cannonTemplate.at(0) = base::loadCannonFromJson(_currentAge, 0);
	_cannonTemplate.at(1) = base::loadCannonFromJson(_currentAge, 1);
	_cannonTemplate.at(2) = base::loadCannonFromJson(_currentAge, 2);
	auto bt0 = core::Renderer::getInstance().findAndRemove("Button");
	if (bt0 == nullptr)
	{
		bt0 = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
		bt0->setTag("Button");
	}
	auto texture = core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/" + _currentAge + "/1.png");
	bt0->setPosition(sf::Vector2f(300.f, 50.f));
	if(texture != nullptr)
	{
		auto ptr = static_cast<Button*>(bt0.get());

		auto& info = ptr->getInfo();
		info.clear();
		info.setPosition(sf::Vector2f(0.f, 300.f));
		info.setTexture(*infoBlock);
		info.setFont(*font);
		info.setColor(sf::Color::Yellow);
		info.setCharacterSize(20);
		info.addValue(" ", _mobTemplate.at(0).displayName);
		info.addValue(&_mobTemplate.at(0).hp, "Hp : ");
		info.addValue(&_mobTemplate.at(0).minDMG, "Base dmg : ");
		info.addValue(&_mobTemplate.at(0).maxDMG, "Critical dmg : ");
		info.addValue(&_mobTemplate.at(0).spawnTime, "Recruitment time : ");
		info.addValue(&_mobTemplate.at(0).price, "Price : ");

		info.refresh();
		ptr->setTexture(*texture);
		ptr->setClickEvent([&](bool isPressed)->void {
			if (isPressed && _enableSpawn)
			{
				addToQueue(0);
			}
			});
		core::Renderer::getInstance().addObject(std::move(bt0), base::object_type::gui);
	}
	auto bt1 = core::Renderer::getInstance().findAndRemove("Button");
	if (bt1 == nullptr)
	{
		bt1 = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
		bt1->setTag("Button");
	}
	texture = core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/" + _currentAge + "/2.png");
	bt1->setPosition(sf::Vector2f(400.f, 50.f));
	{
		auto ptr = static_cast<Button*>(bt1.get());
		auto& info = ptr->getInfo();
		info.clear();
		info.setPosition(sf::Vector2f(0.f, 300.f));
		info.setTexture(*infoBlock);
		info.setFont(*font);
		info.setColor(sf::Color::Yellow);
		info.setCharacterSize(20);
		info.addValue(" ", _mobTemplate.at(1).displayName);
		info.addValue(&_mobTemplate.at(1).hp, "Hp : ");
		info.addValue(&_mobTemplate.at(1).minDMG, "Base dmg : ");
		info.addValue(&_mobTemplate.at(1).maxDMG, "Critical dmg : ");
		info.addValue(&_mobTemplate.at(1).range, "Range : ");
		info.addValue(&_mobTemplate.at(1).spawnTime, "Recruitment time : ");
		info.addValue(&_mobTemplate.at(1).price, "Price : ");

		info.refresh();
		ptr->setTexture(*texture);
		ptr->setClickEvent([&](bool isPressed)->void {
			if (isPressed && _enableSpawn)
			{
				addToQueue(1);
			}
			});
		core::Renderer::getInstance().addObject(std::move(bt1), base::object_type::gui);
	}
	auto bt2 = core::Renderer::getInstance().findAndRemove("Button");
	if (bt2 == nullptr)
	{
		bt2 = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
		bt2->setTag("Button");
	}
	texture = core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/" + _currentAge + "/3.png");
	bt2->setPosition(sf::Vector2f(500.f, 50.f));
	if(texture != nullptr)
	{
		auto ptr = static_cast<Button*>(bt2.get());
		
		auto& info = ptr->getInfo();
		info.clear();
		info.setPosition(sf::Vector2f(0.f, 300.f));
		info.setTexture(*infoBlock);
		info.setFont(*font);
		info.setColor(sf::Color::Yellow);
		info.setCharacterSize(20);
		info.addValue(" ", _mobTemplate.at(2).displayName);
		info.addValue(&_mobTemplate.at(2).hp, "Hp : ");
		info.addValue(&_mobTemplate.at(2).minDMG, "Base dmg : ");
		info.addValue(&_mobTemplate.at(2).maxDMG, "Critical dmg : ");
		info.addValue(&_mobTemplate.at(2).spawnTime, "Recruitment time : ");
		info.addValue(&_mobTemplate.at(2).price, "Price : ");
		info.refresh();
		ptr->setTexture(*texture);
		ptr->setClickEvent([&](bool isPressed)->void {
			if (isPressed && _enableSpawn)
			{
				addToQueue(2);
			}
			});
		core::Renderer::getInstance().addObject(std::move(bt2), base::object_type::gui);
	}
	auto bt3 = core::Renderer::getInstance().findAndRemove("Button");
	if (bt3 == nullptr)
	{
		bt3 = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
		bt3->setTag("Button");
	}
	texture = core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/" + _currentAge + "/4.png");
	bt3->setPosition(sf::Vector2f(700.f, 50.f));
	if(texture!=nullptr)
	{
		auto ptr = static_cast<Button*>(bt3.get());
		auto& info = ptr->getInfo();
		auto& k = _cannonTemplate.at(0);
		info.clear();
		info.setPosition(sf::Vector2f(0.f, 300.f));
		info.setTexture(*infoBlock);
		info.setFont(*font);
		info.setColor(sf::Color::Yellow);
		info.setCharacterSize(20);
		info.addValue(" ", k.displayName);
		info.addValue(&k.minDMG, "Base dmg : ");
		info.addValue(&k.maxDMG, "Critical dmg : ");
		info.addValue(&k.range, "Range : ");
		info.addValue(&k.reloadTime, "Reload : ");
		info.addValue(&k.price, "Price : ");

		info.refresh();
		ptr->setTexture(*texture);
		ptr->setClickEvent([&,texture](bool isPressed)->void {
			if (_enableSpawn && isPressed)
			{
				_drawCannonPlaces = true;
				_enableSpawn = false;
				std::unique_ptr<CannonSpawner> ptr = std::make_unique<CannonSpawner>();
				ptr->setCallbackOnRelease([&, texture]()->void {
					auto& k = core::Renderer::getInstance().find("Player");
					if (k != nullptr)
					{
						auto ptr = static_cast<Player*>(k.get());
						ptr->spawnCannon(0);
					}
					});
				ptr->setTexture(*texture);
				core::Renderer::getInstance().addObject(std::move(ptr), base::object_type::gui);
			}
			});
		core::Renderer::getInstance().addObject(std::move(bt3), base::object_type::gui);
	}

	auto bt4 = core::Renderer::getInstance().findAndRemove("Button");
	if (bt4 == nullptr)
	{
		bt4 = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
		bt4->setTag("Button");
	}
	texture = core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/" + _currentAge + "/5.png");
	bt4->setPosition(sf::Vector2f(800.f, 50.f));
	if(texture != nullptr)
	{
		auto ptr = static_cast<Button*>(bt4.get());
		auto& info = ptr->getInfo();
		auto& k = _cannonTemplate.at(1);
		info.clear();
		info.setPosition(sf::Vector2f(0.f, 300.f));
		info.setTexture(*infoBlock);
		info.setFont(*font);
		info.setColor(sf::Color::Yellow);
		info.setCharacterSize(20);
		info.addValue(" ", k.displayName);
		info.addValue(&k.minDMG, "Base dmg : ");
		info.addValue(&k.maxDMG, "Critical dmg : ");
		info.addValue(&k.range, "Range : ");
		info.addValue(&k.reloadTime, "Reload : ");
		info.addValue(&k.price, "Price : ");

		info.refresh();
		ptr->setTexture(*texture);
		ptr->setClickEvent([&,texture](bool isPressed)->void {
			if (_enableSpawn && isPressed)
			{
				_drawCannonPlaces = true;
				_enableSpawn = false;
				std::unique_ptr<CannonSpawner> ptr = std::make_unique<CannonSpawner>();
				ptr->setCallbackOnRelease([&, texture]()->void {
					auto& k = core::Renderer::getInstance().find("Player");
					if (k != nullptr)
					{
						auto ptr = static_cast<Player*>(k.get());
						ptr->spawnCannon(1);
					}
					});
				ptr->setTexture(*texture);
				core::Renderer::getInstance().addObject(std::move(ptr), base::object_type::gui);
			}
			});
		core::Renderer::getInstance().addObject(std::move(bt4), base::object_type::gui);
	}

	auto bt5 = core::Renderer::getInstance().findAndRemove("Button");
	if (bt5 == nullptr)
	{
		bt5 = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
		bt5->setTag("Button");
	}
	texture = core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/" + _currentAge + "/6.png");
	bt5->setPosition(sf::Vector2f(900.f, 50.f));
	if(texture != nullptr)
	{
		auto ptr = static_cast<Button*>(bt5.get());
		auto& info = ptr->getInfo();
		auto& k = _cannonTemplate.at(2);
		info.clear();
		info.setPosition(sf::Vector2f(0.f, 300.f));
		info.setTexture(*infoBlock);
		info.setFont(*font);
		info.setColor(sf::Color::Yellow);
		info.setCharacterSize(20);
		info.addValue(" ", k.displayName);
		info.addValue(&k.minDMG, "Base dmg : ");
		info.addValue(&k.maxDMG, "Critical dmg : ");
		info.addValue(&k.range, "Range : ");
		info.addValue(&k.reloadTime, "Reload : ");
		info.addValue(&k.price, "Price : ");

		info.refresh();
		ptr->setTexture(*texture);
		ptr->setClickEvent([&,texture](bool isPressed)->void {
			if (_enableSpawn && isPressed)
			{
				_drawCannonPlaces = true;
				_enableSpawn = false;
				std::unique_ptr<CannonSpawner> ptr = std::make_unique<CannonSpawner>();
				ptr->setCallbackOnRelease([&, texture]()->void {
					auto& k = core::Renderer::getInstance().find("Player");
					if (k != nullptr)
					{
						auto ptr = static_cast<Player*>(k.get());
						ptr->spawnCannon(2);
					}
					});
				ptr->setTexture(*texture);
				core::Renderer::getInstance().addObject(std::move(ptr), base::object_type::gui);
			}
			});
		core::Renderer::getInstance().addObject(std::move(bt5), base::object_type::gui);
	}
}
