#include "Player.hpp"
#include "../Core/ResourceManager.hpp"
#include "../Core/Renderer.hpp"
#include "../Core/Application.hpp"
#include "Melee.hpp"
#include "Range.hpp"
#include "Button.hpp"
#include "QueueManager.hpp"
#include "CannonSpawner.hpp"
#include "Counter.hpp"
#include "HpBar.hpp"

Player::Player()
	:_enableSpawn(true), _drawCannonPlaces(false), _coinCount(10000), _wantSell(false), _sellClickCount(0), _baseUpgrade(1), _expCount(1), _upgradeCondition(1000), _cannonPlaceCost(1000)
{
	_cannons = { nullptr,nullptr,nullptr };
	_tag = "Player";
	_currentAge = "0";
	_timer = 0;
	_sprites.at(0).setOrigin(275.f, 170.f);
	_myColider = { 111.f,111.f,111.f,111.f };

	_ages = { "I","II","III","IV","V" };
	auto hp = std::make_unique<HpBar>(&_hp, 24, 250);
	hp->setPosition(sf::Vector2f(20.f, 800));
	hp->setCallbackOnDie([&]()->void {
		auto tex = core::ResourceManager<sf::Texture>::getInstance().get("Assets/frames/2.png");
		core::Application::getInstance().freezeScreen(*tex);
		});
	hp->setFont(*core::ResourceManager<sf::Font>::getInstance().get("Assets/fonts/3.ttf"));
	hp->setCharacterSize(15);
	core::Renderer::getInstance().addObject(std::move(hp), base::object_type::gui);

	for (auto& k : _cannonPlaces)
	{
		k.setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/base/6.png"));
		k.setOrigin(sf::Vector2f(30.f, 30.f));
	}

	auto manager = std::make_unique<QueueManager>();
	manager->setPosition(sf::Vector2f(300.f, 150.f));
	manager->dynamicDraw(false);
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
	button->dynamicDraw(false);
	button->setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/2.png"));

	button->setPosition(sf::Vector2f(1100, 50));
	core::Renderer::getInstance().addObject(std::move(button), base::object_type::gui);

	auto& infoBlock = core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/5.png");
	auto& font = core::ResourceManager<sf::Font>::getInstance().get("Assets/fonts/3.ttf");
	{
		button = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
		button->setTag("AgeUpgradeButton");
		button->dynamicDraw(false);
		button->setClickEvent([&](bool isPressed)->void {
			if (isPressed && _enableSpawn) {
				_enableSpawn = false;
				this->loadNextAge();
			}
			});

		auto ptr = static_cast<Button*>(button.get());
		auto& info = ptr->getInfo();
		auto& k = _cannonTemplate.at(1);
		info.clear();
		info.setPosition(sf::Vector2f(0.f, 300.f));
		info.setTexture(*infoBlock);
		info.setFont(*font);
		info.setColor(sf::Color::Yellow);
		info.setCharacterSize(15);
		info.addValue(" ", "\nResearch new ages and \nlearn modern tech\n ");
		info.addValue(&_upgradeCondition, "Exp points required to upgrade\n  : ");

	}
	button->setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/1.png"));

	button->setPosition(sf::Vector2f(1300, 50));
	core::Renderer::getInstance().addObject(std::move(button), base::object_type::gui);
	button = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
	button->dynamicDraw(false);
	button->setTag("BaseUpgradeButton");
	button->setClickEvent([&](bool isPressed)->void {
		if (isPressed && _enableSpawn) {
			_enableSpawn = false;
			this->baseUpgrade();
		}
		});
	{
		auto ptr = static_cast<Button*>(button.get());
		auto& info = ptr->getInfo();
		auto& k = _cannonTemplate.at(1);
		info.clear();
		info.setPosition(sf::Vector2f(0.f, 300.f));
		info.setTexture(*infoBlock);
		info.setFont(*font);
		info.setColor(sf::Color::Yellow);
		info.setCharacterSize(15);
		info.addValue(" ", "\nCreate new cannon place\n ");
		info.addValue(&_cannonPlaceCost, "Cost : ");
	}


	button->setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/7.png"));

	button->setPosition(sf::Vector2f(1200, 50));
	core::Renderer::getInstance().addObject(std::move(button), base::object_type::gui);

	loadNextAge();

	std::unique_ptr<Counter> coin = std::make_unique<Counter>(&_coinCount);
	coin->setPosition(sf::Vector2f(1600, 40));
	coin->setFont(*core::ResourceManager<sf::Font>::getInstance().get("Assets/fonts/3.ttf"));
	coin->setCharacterSize(20);
	coin->dynamicDraw(false);
	coin->setColor(sf::Color::Yellow);
	auto ck = core::Application::getInstance().getClip("coin");
	coin->addClip(std::move(ck), "coin");
	core::Renderer::getInstance().addObject(std::move(coin), base::object_type::gui);

	std::unique_ptr<Counter> exp = std::make_unique<Counter>(&_expCount);
	exp->setPosition(sf::Vector2f(1600, 80));
	exp->setFont(*core::ResourceManager<sf::Font>::getInstance().get("Assets/fonts/3.ttf"));
	exp->setCharacterSize(20);
	exp->dynamicDraw(false);
	exp->setColor(sf::Color::Yellow);
	auto ek = core::Application::getInstance().getClip("star");
	exp->addClip(std::move(ek), "star");
	core::Renderer::getInstance().addObject(std::move(exp), base::object_type::gui);

}

void Player::setPosition(const sf::Vector2f& pos)
{
	_position = pos;
	_sprites.at(0).setPosition(pos);

	for (int k = 0; k < 3; ++k)
	{
		_cannonPlaces[k].setPosition(sf::Vector2f(pos.x + 10, pos.y - 70.f - 70.f * k));
		if (_cannons[k] != nullptr)
			_cannons[k]->setPosition(sf::Vector2f(pos.x + 10, pos.y - 70.f - 70.f * k));

		if (k < 2)
		{
			_sprites.at(k + 1).setPosition(pos.x + 10, pos.y -50.f - 90.f * (k + 1));
		}
	}
}

void Player::setScale(const sf::Vector2f& sca)
{
	_scale = sca;
	for(auto& k : _sprites)
		k.setScale(sca);
	for (int k = 0; k < 3; ++k)
	{
		_cannonPlaces[k].scale(sca);
		_cannons[k]->setScale(sca);
	}
}

void Player::move(const sf::Vector2f& delta)
{
	_position += delta;

	for(auto& k : _sprites)
		k.move(delta);

	for (auto& k : _cannons)
	{
		if (k != nullptr)
			k->move(delta);
	}
	
	for(auto& k : _cannonPlaces)
		k.move(delta);
}

void Player::onUpdate()
{
	if (_timer >= 0.3f)
	{
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			_enableSpawn = true;
			if(!_wantSell)
				_drawCannonPlaces = false;
		}
	}
	else _timer += core::Application::getInstance().getTime();

	{
		for (int k = 0; k < _baseUpgrade; ++k)
		{
			if(_cannons[k] != nullptr)
				_cannons[k]->onUpdate();
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (_wantSell)
			{
				++_sellClickCount;
				if (_sellClickCount > 1)
				{
					for (int k = 0; k < _baseUpgrade; ++k)
					{
						if (_cannons[k] == nullptr)
							continue;
						auto pos = _cannons[k]->getPosition();
						auto mPos = core::Renderer::getInstance().getMousePosition();
						if (pos.x + 30.f > mPos.x && pos.x - 30 < mPos.x)
						{
							if (pos.y - 30 < mPos.y && pos.y + 30.f > mPos.y)
							{
								_coinCount += _cannons[k]->getSellPrice();
								_cannons[k] = nullptr;
								break;
							}
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
	for(int k = 0; k<_baseUpgrade; ++k)
	{
		target.draw(_sprites[k], states);
	}
	for (int k = 0; k < _baseUpgrade; ++k)
	{
		if (_cannons[k] != nullptr)
		{
			target.draw(*_cannons[k], states);
			if (_wantSell)
				target.draw(_cannonPlaces[k], states);
		}
		else if (_drawCannonPlaces)
		{
			target.draw(_cannonPlaces[k], states);
		}
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
	if (_cannonTemplate.at(type).price > _coinCount)
		return;

	auto mPos = core::Renderer::getInstance().getMousePosition();

	for (int v = 0; v < _baseUpgrade; ++v)
	{
		if (_cannons[v] == nullptr)
		{
			auto pos = _cannonPlaces[v].getPosition();
			if (pos.x + 30.f > mPos.x && pos.x - 30 < mPos.x)
			{
				if (pos.y - 30 < mPos.y && pos.y + 30.f > mPos.y)
				{
					auto& k = _cannonTemplate.at(type);
					std::string name = _currentAge + "C" + std::to_string(type);
					_cannons.at(v) = std::make_unique<Cannon>(k.maxDMG, k.minDMG, k.range, k.reloadTime, k.bulletSpeed, k.bulletPosition, k.fireSpeed, k.price / 2);
					if (k.long_range)
						_cannons.at(v)->longRange();
					auto clip = core::Application::getInstance().getClip(name);
					_cannons.at(v)->addClip(std::move(clip), name);
					name = "Assets/ammunition/" + _currentAge + "/C" + std::to_string(type) + ".png";
					_cannons.at(v)->setBulletTexture(core::ResourceManager<sf::Texture>::getInstance().get(name));
					_cannons.at(v)->setAnimationSpeed(k.animationSpeed);
					_cannons.at(v)->setPosition(_cannonPlaces.at(v).getPosition());
					_coinCount -= k.price;
					return;
				}
			}
		}
	}
}

void Player::sellActive()
{
	for (int k = 0; k < _baseUpgrade; ++k)
	{
		if (_cannons.at(k) != nullptr)
		{
			_drawCannonPlaces = true;
			_wantSell = true;
			core::Application::getInstance().setCursor(1);
			break;
		}
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
	if (_currentAge == _ages.back())
	{
		return;
	}
	if (_currentAge == "0") {
		_currentAge = "I";
	}
	else {
		for (auto k = _ages.begin(); k != _ages.end(); ++k)
		{
			if ((*k) == _currentAge)
			{
				++k;
				if (k != _ages.end()) {
					_currentAge = *k;
				}
			}
		}
	}
	if (_currentAge == _ages.back())
	{
		auto& k = core::Renderer::getInstance().find("AgeUpgradeButton");
		k->remove();
		core::Renderer::getInstance().clearNoActive();
	}
	_upgradeCondition *= 2;

	auto& infoBlock = core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/5.png");
	auto& font = core::ResourceManager<sf::Font>::getInstance().get("Assets/fonts/3.ttf");

	_sprites.at(0).setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/base/" + _currentAge + ".png"), true);
	_sprites.at(1).setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/base/" + _currentAge + "1.png"), true);
	_sprites.at(2).setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/base/" + _currentAge + "2.png"),true);
	_sprites.at(1).setOrigin(sf::Vector2f(88.f, 99.f));
	_sprites.at(2).setOrigin(sf::Vector2f(88.f, 99.f));
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
	bt0->dynamicDraw(false);
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
	bt1->dynamicDraw(false);
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
	bt2->dynamicDraw(false);
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
	bt3->dynamicDraw(false);
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
	bt4->dynamicDraw(false);
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
	bt5->dynamicDraw(false);
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

void Player::baseUpgrade()
{
	if (_baseUpgrade < static_cast<int>(_sprites.size()))
	{
		if (_coinCount > _cannonPlaceCost)
		{
			_coinCount -= _cannonPlaceCost;
			++_baseUpgrade;
			_cannonPlaceCost *= 2;
			if (_baseUpgrade == static_cast<int>(_sprites.size()))
			{
				auto& k = core::Renderer::getInstance().find("BaseUpgradeButton");
				k->remove();
				core::Renderer::getInstance().clearNoActive();
			}
		}
	}
}
