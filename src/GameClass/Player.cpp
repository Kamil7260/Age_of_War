#include "Player.hpp"
#include "../Core/ResourceManager.hpp"
#include "../Core/Renderer.hpp"
#include "../Core/Application.hpp"
#include "Melee.hpp"
#include "Range.hpp"
#include "QueueManager.hpp"
#include "CannonSpawner.hpp"
#include "Counter.hpp"
#include "HpBar.hpp"
#include "UpgradeButton.hpp"

Player::Player()
	:m_enableSpawn(true), m_drawCannonPlaces(false), m_coinCount(250), m_wantSell(false), m_sellClickCount(0), m_baseUpgrade(1), m_expCount(1), m_upgradeCondition(750), m_cannonPlaceCost(700), m_enableInsertToQueue(true)
{
	m_cannons = { nullptr,nullptr,nullptr };
	m_tag = "Player";
	m_currentAge = "0";
	m_timer = 0;
	m_sprites.at(0).setOrigin(275.f, 170.f);
	m_myColider = { 111.f,111.f,111.f,111.f };

	m_unitUpgradeCost = { 300,400,700 };

	m_ages = { "I","II","III","IV","V" };
	auto hp = std::make_unique<HpBar>(&m_hp, 24, 250);
	hp->setPosition(sf::Vector2f(20.f, 800));
	hp->setCallbackOnDie([&]()->void {
		auto tex = core::ResourceManager<sf::Texture>::getInstance().get("Assets/frames/2.png");
		core::Application::getInstance().freezeScreen(*tex);
		});
	hp->setFont(*core::ResourceManager<sf::Font>::getInstance().get("Assets/fonts/3.ttf"));
	hp->setCharacterSize(15);
	core::Renderer::getInstance().addObject(std::move(hp), base::object_type::gui);

	for (auto& k : m_cannonPlaces)
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
	
	button = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
	button->dynamicDraw(false);
	button->setTag("BaseUpgradeButton");
	button->setClickEvent([&](bool isPressed)->void {
		if (isPressed && m_enableSpawn) {
			m_enableSpawn = false;
			this->baseUpgrade();
		}
		});
	{
		auto ptr = static_cast<Button*>(button.get());
		auto& info = ptr->getInfo();
		auto& k = m_cannonTemplate.at(1);
		info.clear();
		info.setPosition(sf::Vector2f(0.f, 300.f));
		info.setTexture(*infoBlock);
		info.setFont(*font);
		info.setColor(sf::Color::Yellow);
		info.setCharacterSize(15);
		info.addValue(" ", "\nCreate new cannon place\n ");
		info.addValue(&m_cannonPlaceCost, "Cost : ");
	}


	button->setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/7.png"));

	button->setPosition(sf::Vector2f(1200, 50));
	core::Renderer::getInstance().addObject(std::move(button), base::object_type::gui);

	loadNextAge();

	std::unique_ptr<Counter> coin = std::make_unique<Counter>(&m_coinCount);
	coin->setPosition(sf::Vector2f(1600, 40));
	coin->setFont(*core::ResourceManager<sf::Font>::getInstance().get("Assets/fonts/3.ttf"));
	coin->setCharacterSize(20);
	coin->dynamicDraw(false);
	coin->setColor(sf::Color::Yellow);
	auto ck = core::Application::getInstance().getClip("coin");
	coin->addClip(std::move(ck), "coin");
	core::Renderer::getInstance().addObject(std::move(coin), base::object_type::gui);

	std::unique_ptr<Counter> exp = std::make_unique<Counter>(&m_expCount);
	exp->setPosition(sf::Vector2f(1600, 80));
	exp->setFont(*core::ResourceManager<sf::Font>::getInstance().get("Assets/fonts/3.ttf"));
	exp->setCharacterSize(20);
	exp->dynamicDraw(false);
	exp->setColor(sf::Color::Yellow);
	auto ek = core::Application::getInstance().getClip("star");
	exp->addClip(std::move(ek), "star");
	core::Renderer::getInstance().addObject(std::move(exp), base::object_type::gui);


	auto upgrade = std::make_unique<UpgradeButton>(base::collider({ 0,50,0,50 }));
	upgrade->dynamicDraw(false);
	auto& info1 = upgrade->getInfo();
	info1.clear();
	info1.setPosition(sf::Vector2f(0.f, 300.f));
	info1.setTexture(*infoBlock);
	info1.setFont(*font);
	info1.setColor(sf::Color::Yellow);
	info1.setCharacterSize(20);
	info1.addValue<std::string>(nullptr,"Upgrade unit");
	info1.addValue(&m_unitUpgradeCost.at(0),"Cost ");

	upgrade->setTag("UpgradeButton1");
	upgrade->setPosition(sf::Vector2f(280, 0));
	upgrade->setClickEvent([&](bool isPressed)->void {
		if (isPressed && m_enableSpawn)
		{
			m_enableSpawn = false;
			if (m_coinCount < m_unitUpgradeCost.at(0))
				return;
			auto& k = core::Renderer::getInstance().find("UpgradeButton1");
			if (k != nullptr)
			{
				auto ptr = static_cast<UpgradeButton*>(k.get());
				ptr->increaseLvl();
				this->upgradeUnit(0);
			}
		}
		});
	upgrade->addLevel(core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/up/1.png"));
	upgrade->addLevel(core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/up/2.png"));
	upgrade->addLevel(core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/up/3.png"));
	upgrade->addLevel(core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/up/4.png"));
	upgrade->addLevel(core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/up/5.png"));
	upgrade->addLevel(core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/up/6.png"));

	core::Renderer::getInstance().addObject(std::move(upgrade), base::object_type::gui);

	upgrade = std::make_unique<UpgradeButton>(base::collider({ 0,50,0,50 }));
	upgrade->setTag("UpgradeButton2");
	upgrade->setPosition(sf::Vector2f(390, 0));
	upgrade->dynamicDraw(false);
	auto& info2 = upgrade->getInfo();
	info2.clear();
	info2.setPosition(sf::Vector2f(0.f, 300.f));
	info2.setTexture(*infoBlock);
	info2.setFont(*font);
	info2.setColor(sf::Color::Yellow);
	info2.setCharacterSize(20);
	info2.addValue<std::string>(nullptr, "Upgrade unit");
	info2.addValue(&m_unitUpgradeCost.at(1), "Cost ");

	upgrade->setClickEvent([&](bool isPressed)->void {
		if (isPressed && m_enableSpawn)
		{
			if (m_coinCount < m_unitUpgradeCost.at(1))
				return;
			m_enableSpawn = false;
			auto& k = core::Renderer::getInstance().find("UpgradeButton2");
			if (k != nullptr)
			{
				auto ptr = static_cast<UpgradeButton*>(k.get());
				ptr->increaseLvl();
				this->upgradeUnit(1);
			}
		}
		});
	upgrade->addLevel(core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/up/1.png"));
	upgrade->addLevel(core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/up/2.png"));
	upgrade->addLevel(core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/up/3.png"));
	upgrade->addLevel(core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/up/4.png"));
	upgrade->addLevel(core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/up/5.png"));
	upgrade->addLevel(core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/up/6.png"));

	core::Renderer::getInstance().addObject(std::move(upgrade), base::object_type::gui);


	upgrade = std::make_unique<UpgradeButton>(base::collider({ 0,50,0,50 }));
	upgrade->setTag("UpgradeButton3");
	upgrade->setPosition(sf::Vector2f(500, 0));
	upgrade->dynamicDraw(false);
	auto& info3 = upgrade->getInfo();
	info3.clear();
	info3.setPosition(sf::Vector2f(0.f, 300.f));
	info3.setTexture(*infoBlock);
	info3.setFont(*font);
	info3.setColor(sf::Color::Yellow);
	info3.setCharacterSize(20);
	info3.addValue<std::string>(nullptr, "Upgrade unit");
	info3.addValue(&m_unitUpgradeCost.at(2), "Cost ");

	upgrade->setClickEvent([&](bool isPressed)->void {
		if (isPressed && m_enableSpawn)
		{
			if (m_coinCount < m_unitUpgradeCost.at(2))
				return;

			m_enableSpawn = false;
			auto& k = core::Renderer::getInstance().find("UpgradeButton3");
			if (k != nullptr)
			{
				auto ptr = static_cast<UpgradeButton*>(k.get());
				ptr->increaseLvl();
				this->upgradeUnit(2);
			}
		}
		});
	upgrade->addLevel(core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/up/1.png"));
	upgrade->addLevel(core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/up/2.png"));
	upgrade->addLevel(core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/up/3.png"));
	upgrade->addLevel(core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/up/4.png"));
	upgrade->addLevel(core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/up/5.png"));
	upgrade->addLevel(core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/up/6.png"));

	core::Renderer::getInstance().addObject(std::move(upgrade), base::object_type::gui);
}

void Player::setPosition(const sf::Vector2f& pos)
{
	m_position = pos;
	m_sprites.at(0).setPosition(pos);

	for (int k = 0; k < 3; ++k)
	{
		m_cannonPlaces[k].setPosition(sf::Vector2f(pos.x + 10, pos.y - 70.f - 70.f * k));
		if (m_cannons[k] != nullptr)
			m_cannons[k]->setPosition(sf::Vector2f(pos.x + 10, pos.y - 70.f - 70.f * k));

		if (k < 2)
		{
			m_sprites.at(k + 1).setPosition(pos.x + 10, pos.y -50.f - 90.f * (k + 1));
		}
	}
}

void Player::setScale(const sf::Vector2f& sca)
{
	m_scale = sca;
	for(auto& k : m_sprites)
		k.setScale(sca);
	for (int k = 0; k < 3; ++k)
	{
		m_cannonPlaces[k].scale(sca);
		m_cannons[k]->setScale(sca);
	}
}

void Player::move(const sf::Vector2f& delta)
{
	m_position += delta;

	for(auto& k : m_sprites)
		k.move(delta);

	for (auto& k : m_cannons)
	{
		if (k != nullptr)
			k->move(delta);
	}
	
	for(auto& k : m_cannonPlaces)
		k.move(delta);
}

void Player::onUpdate()
{
	if (m_timer >= 0.3f)
	{
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			m_enableSpawn = true;
			if(!m_wantSell)
				m_drawCannonPlaces = false;
		}
	}
	else m_timer += core::Application::getInstance().getTime();

	{
		for (int k = 0; k < m_baseUpgrade; ++k)
		{
			if(m_cannons[k] != nullptr)
				m_cannons[k]->onUpdate();
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (m_wantSell)
			{
				++m_sellClickCount;
				if (m_sellClickCount > 1)
				{
					for (int k = 0; k < m_baseUpgrade; ++k)
					{
						if (m_cannons[k] == nullptr)
							continue;
						auto pos = m_cannons[k]->getPosition();
						auto mPos = core::Renderer::getInstance().getMousePosition();
						if (pos.x + 30.f > mPos.x && pos.x - 30 < mPos.x)
						{
							if (pos.y - 30 < mPos.y && pos.y + 30.f > mPos.y)
							{
								m_coinCount += m_cannons[k]->getSellPrice();
								m_cannons[k] = nullptr;
								break;
							}
						}
					}
					m_sellClickCount = 0;
					m_wantSell = false;
					m_drawCannonPlaces = false;
					sellCanceled();

				}
			}
			else {
				m_sellClickCount = 0;
			}
		}
	}
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(int k = 0; k<m_baseUpgrade; ++k)
	{
		target.draw(m_sprites[k], states);
	}
	for (int k = m_baseUpgrade -1; k >= 0; --k)
	{
		if (m_cannons[k] != nullptr)
		{
			target.draw(*m_cannons[k], states);
			if (m_wantSell)
				target.draw(m_cannonPlaces[k], states);
		}
		else if (m_drawCannonPlaces)
		{
			target.draw(m_cannonPlaces[k], states);
		}
	}
}


void Player::spawnObject(const unsigned int type)
{
	if (type == 1)
	{
		auto& k = m_calcMobTemplate.at(type);
		std::unique_ptr<Range> man = std::make_unique<Range>(k.collider, k.hp, k.minDMG, k.maxDMG,k.range,k.speedAttack, k.speedMove,k.income);
		man->setScale(sf::Vector2f(k.scale.x, k.scale.y));
		man->setAnimatorName(k.name);
		man->setAnimationSpeed(k.animationSpeed);
		man->setPosition(sf::Vector2f(m_position.x + 100, m_position.y + 75));
		core::Renderer::getInstance().addObject(std::move(man), base::object_type::actor);
		return;
	}
	auto& k = m_calcMobTemplate.at(type);
	std::unique_ptr<Melee> man = std::make_unique<Melee>(k.collider, k.hp,k.minDMG, k.maxDMG, k.speedAttack, k.speedMove, k.income);
	man->setScale(sf::Vector2f(k.scale.x, k.scale.y));
	man->setAnimatorName(k.name);
	man->setAnimationSpeed(k.animationSpeed);
	man->setPosition(sf::Vector2f(m_position.x + 100, m_position.y + 75));
	core::Renderer::getInstance().addObject(std::move(man), base::object_type::actor);
}

void Player::addToQueue(const unsigned int type)
{
	if (m_mobTemplate.at(type).price > m_coinCount)
		return;

	m_coinCount -=m_mobTemplate.at(type).price;
	m_timer = 0.f;
	m_enableSpawn = false;
	auto& manager = core::Renderer::getInstance().find("QueueManager");
	if (manager != nullptr)
	{
		auto queue = static_cast<QueueManager*>(manager.get());
		Ikon ikon;
		ikon.setTimePoint(m_mobTemplate.at(type).spawnTime);
		std::string path = "Assets/gui/" + m_currentAge + "/" + std::to_string(type + 1) + ".png";
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
	if (m_cannonTemplate.at(type).price > m_coinCount)
		return;

	auto mPos = core::Renderer::getInstance().getMousePosition();

	for (int v = 0; v < m_baseUpgrade; ++v)
	{
		if (m_cannons[v] == nullptr)
		{
			auto pos = m_cannonPlaces[v].getPosition();
			if (pos.x + 30.f > mPos.x && pos.x - 30 < mPos.x)
			{
				if (pos.y - 30 < mPos.y && pos.y + 30.f > mPos.y)
				{
					auto& k = m_cannonTemplate.at(type);
					std::string name = m_currentAge + "C" + std::to_string(type);
					m_cannons.at(v) = std::make_unique<Cannon>(k.maxDMG, k.minDMG, k.range, k.reloadTime, k.bulletSpeed, k.bulletPosition, k.fireSpeed, k.price / 2);
					if (k.long_range)
						m_cannons.at(v)->longRange();
					auto clip = core::Application::getInstance().getClip(name);
					m_cannons.at(v)->addClip(std::move(clip), name);
					name = "Assets/ammunition/" + m_currentAge + "/C" + std::to_string(type) + ".png";
					m_cannons.at(v)->setBulletTexture(core::ResourceManager<sf::Texture>::getInstance().get(name));
					m_cannons.at(v)->setAnimationSpeed(k.animationSpeed);
					m_cannons.at(v)->setPosition(m_cannonPlaces.at(v).getPosition());
					m_coinCount -= k.price;
					return;
				}
			}
		}
	}
}

void Player::sellActive()
{
	for (int k = 0; k < m_baseUpgrade; ++k)
	{
		if (m_cannons.at(k) != nullptr)
		{
			m_drawCannonPlaces = true;
			m_wantSell = true;
			core::Application::getInstance().setCursor(1);
			break;
		}
	}
	m_sellClickCount = 0;
}

void Player::sellCanceled()
{
	m_wantSell = false;
	core::Application::getInstance().setCursor(0);
}

void Player::loadNextAge()
{

	if (m_currentAge == m_ages.back())
	{
		return;
	}
	if (m_currentAge == "0") {
		m_currentAge = "I";
	}
	else {
	
		if (m_expCount < m_upgradeCondition)
			return;
		for (auto k = m_ages.begin(); k != m_ages.end(); ++k)
		{
			if ((*k) == m_currentAge)
			{
				++k;
				if (k != m_ages.end()) {
					m_currentAge = *k;
				}
			}
		}
	}
	m_upgradeCondition *= 2;


	auto& infoBlock = core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/5.png");
	auto& font = core::ResourceManager<sf::Font>::getInstance().get("Assets/fonts/3.ttf");

	if (m_currentAge != m_ages.back())
	{
		auto button = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
		button->setTag("AgeUpgradeButton");
		button->dynamicDraw(false);

		button->setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/1.png"));

		button->setPosition(sf::Vector2f(1300, 50));

		button->setClickEvent([&](bool isPressed)->void {
			if (isPressed && m_enableSpawn) {
				if (m_expCount < m_upgradeCondition)
					return;

				m_enableInsertToQueue = false;
				m_timer = 0.f;
				m_enableSpawn = false;
				auto& manager = core::Renderer::getInstance().find("QueueManager");
				if (manager != nullptr)
				{
					auto& b = core::Renderer::getInstance().find("AgeUpgradeButton");
					b->remove();
					core::Renderer::getInstance().clearNoActive();
					auto queue = static_cast<QueueManager*>(manager.get());
					Ikon ikon;
					ikon.setTimePoint(15.f);
					std::string path = "Assets/gui/1.png";
					ikon.setTexture(*core::ResourceManager<sf::Texture>::getInstance().get(path));
					queue->addIkon(ikon, [&]()->void {
						m_enableInsertToQueue = true;
						this->loadNextAge();
						});
				}
				else {
					LOG_ERROR("Can not find QueueManager");
				}
			}
			});

		auto ptr = static_cast<Button*>(button.get());
		auto& info = ptr->getInfo();
		auto& k = m_cannonTemplate.at(1);
		info.clear();
		info.setPosition(sf::Vector2f(0.f, 300.f));
		info.setTexture(*infoBlock);
		info.setFont(*font);
		info.setColor(sf::Color::Yellow);
		info.setCharacterSize(15);
		info.addValue(" ", "\nResearch new ages and \nlearn modern tech\n ");
		info.addValue(&m_upgradeCondition, "Exp points required to upgrade\n  : ");

		core::Renderer::getInstance().addObject(std::move(button), base::object_type::gui);
	}

	m_sprites.at(0).setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/base/" + m_currentAge + ".png"), true);
	m_sprites.at(1).setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/base/" + m_currentAge + "1.png"), true);
	m_sprites.at(2).setTexture(*core::ResourceManager<sf::Texture>::getInstance().get("Assets/base/" + m_currentAge + "2.png"),true);
	m_sprites.at(1).setOrigin(sf::Vector2f(88.f, 99.f));
	m_sprites.at(2).setOrigin(sf::Vector2f(88.f, 99.f));
	m_calcMobTemplate.at(0) = m_mobTemplate.at(0) = base::loadUnitFromJson(0, m_currentAge);
	m_calcMobTemplate.at(1) = m_mobTemplate.at(1) = base::loadUnitFromJson(1, m_currentAge);
	m_calcMobTemplate.at(2) = m_mobTemplate.at(2) = base::loadUnitFromJson(2, m_currentAge);
	m_cannonTemplate.at(0) = base::loadCannonFromJson(m_currentAge, 0);
	m_cannonTemplate.at(1) = base::loadCannonFromJson(m_currentAge, 1);
	if (m_currentAge != "II")
		m_cannonTemplate.at(2) = base::loadCannonFromJson(m_currentAge, 2);

	refreshStats();

	auto bt0 = core::Renderer::getInstance().findAndRemove("Button");
	if (bt0 == nullptr)
	{
		bt0 = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
		bt0->setTag("Button");
	}
	auto texture = core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/" + m_currentAge + "/1.png");
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
		info.addValue(" ", m_calcMobTemplate.at(0).displayName);
		info.addValue(&m_calcMobTemplate.at(0).hp, "Hp : ");
		info.addValue(&m_calcMobTemplate.at(0).minDMG, "Base dmg : ");
		info.addValue(&m_calcMobTemplate.at(0).maxDMG, "Critical dmg : ");
		info.addValue(&m_calcMobTemplate.at(0).spawnTime, "Recruitment time : ");
		info.addValue(&m_calcMobTemplate.at(0).price, "Price : ");

		info.refresh();
		ptr->setTexture(*texture);
		ptr->setClickEvent([&](bool isPressed)->void {
			if (isPressed && m_enableSpawn)
			{
				if(m_enableInsertToQueue)
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
	texture = core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/" + m_currentAge + "/2.png");
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
		info.addValue(" ", m_calcMobTemplate.at(1).displayName);
		info.addValue(&m_calcMobTemplate.at(1).hp, "Hp : ");
		info.addValue(&m_calcMobTemplate.at(1).minDMG, "Base dmg : ");
		info.addValue(&m_calcMobTemplate.at(1).maxDMG, "Critical dmg : ");
		info.addValue(&m_calcMobTemplate.at(1).range, "Range : ");
		info.addValue(&m_calcMobTemplate.at(1).spawnTime, "Recruitment time : ");
		info.addValue(&m_calcMobTemplate.at(1).price, "Price : ");

		info.refresh();
		ptr->setTexture(*texture);
		ptr->setClickEvent([&](bool isPressed)->void {
			if (isPressed && m_enableSpawn)
			{
				if(m_enableInsertToQueue)
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
	texture = core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/" + m_currentAge + "/3.png");
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
		info.addValue(" ", m_calcMobTemplate.at(2).displayName);
		info.addValue(&m_calcMobTemplate.at(2).hp, "Hp : ");
		info.addValue(&m_calcMobTemplate.at(2).minDMG, "Base dmg : ");
		info.addValue(&m_calcMobTemplate.at(2).maxDMG, "Critical dmg : ");
		info.addValue(&m_calcMobTemplate.at(2).spawnTime, "Recruitment time : ");
		info.addValue(&m_calcMobTemplate.at(2).price, "Price : ");
		info.refresh();
		ptr->setTexture(*texture);
		ptr->setClickEvent([&](bool isPressed)->void {
			if (isPressed && m_enableSpawn)
			{
				if(m_enableInsertToQueue)
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
	texture = core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/" + m_currentAge + "/4.png");
	bt3->setPosition(sf::Vector2f(700.f, 50.f));
	bt3->dynamicDraw(false);
	if(texture!=nullptr)
	{
		auto ptr = static_cast<Button*>(bt3.get());
		auto& info = ptr->getInfo();
		auto& k = m_cannonTemplate.at(0);
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
			if (m_enableSpawn && isPressed)
			{
				m_drawCannonPlaces = true;
				m_enableSpawn = false;
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
	texture = core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/" + m_currentAge + "/5.png");
	bt4->setPosition(sf::Vector2f(800.f, 50.f));
	bt4->dynamicDraw(false);
	if(texture != nullptr)
	{
		auto ptr = static_cast<Button*>(bt4.get());
		auto& info = ptr->getInfo();
		auto& k = m_cannonTemplate.at(1);
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
			if (m_enableSpawn && isPressed)
			{
				m_drawCannonPlaces = true;
				m_enableSpawn = false;
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
	if (m_currentAge != "II")
	{
		if (bt5 == nullptr)
		{
			bt5 = std::make_unique<Button>(base::collider({ 0.f,68.f,0.f,68.f }));
			bt5->setTag("Button");
		}
		texture = core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/" + m_currentAge + "/6.png");
		bt5->setPosition(sf::Vector2f(900.f, 50.f));
		bt5->dynamicDraw(false);
		if (texture != nullptr)
		{
			auto ptr = static_cast<Button*>(bt5.get());
			auto& info = ptr->getInfo();
			auto& k = m_cannonTemplate.at(2);
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
			ptr->setClickEvent([&, texture](bool isPressed)->void {
				if (m_enableSpawn && isPressed)
				{
					m_drawCannonPlaces = true;
					m_enableSpawn = false;
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
}

void Player::baseUpgrade()
{
	if (m_baseUpgrade < static_cast<int>(m_sprites.size()))
	{
		if (m_coinCount > m_cannonPlaceCost)
		{
			m_coinCount -= m_cannonPlaceCost;
			++m_baseUpgrade;
			m_cannonPlaceCost *= 2;
			if (m_baseUpgrade == static_cast<int>(m_sprites.size()))
			{
				auto& k = core::Renderer::getInstance().find("BaseUpgradeButton");
				k->remove();
				core::Renderer::getInstance().clearNoActive();
			}
		}
	}
}

void Player::refreshStats()
{
	for (size_t k = 0; k < m_calcMobTemplate.size(); ++k)
	{
		m_calcMobTemplate.at(k).hp = static_cast<int>(m_mobTemplate.at(k).hp * m_upgrades.at(k).hp);
		m_calcMobTemplate.at(k).range = static_cast<int>(m_mobTemplate.at(k).range * m_upgrades.at(k).range);
		m_calcMobTemplate.at(k).maxDMG = static_cast<int>(m_mobTemplate.at(k).maxDMG * m_upgrades.at(k).maxDMG);
		m_calcMobTemplate.at(k).minDMG = static_cast<int>(m_mobTemplate.at(k).minDMG * m_upgrades.at(k).minDMG);
	}
}

void Player::upgradeUnit(const unsigned int index)
{	
	if (index < m_upgrades.size())
	{
		m_coinCount -= m_unitUpgradeCost.at(index);
		m_unitUpgradeCost.at(index) *= 2;
		auto& k = m_upgrades.at(index);
		k.hp += 0.05f;
		k.maxDMG += 0.05f;
		k.minDMG += 0.05f;
		k.range += 0.15f;
		m_calcMobTemplate.at(index).hp = static_cast<int>(m_mobTemplate.at(index).hp * k.hp);
		m_calcMobTemplate.at(index).range = static_cast<int>(m_mobTemplate.at(index).range * k.range);
		m_calcMobTemplate.at(index).maxDMG = static_cast<int>(m_mobTemplate.at(index).maxDMG * k.maxDMG);
		m_calcMobTemplate.at(index).minDMG = static_cast<int>(m_mobTemplate.at(index).minDMG * k.minDMG);
	}
}
