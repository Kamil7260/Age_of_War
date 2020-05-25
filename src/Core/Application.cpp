#include <string>
#include <fstream>
#include "Application.hpp"
#include "ResourceManager.hpp"

#include "../GameClass/Background.hpp"
#include "../GameClass/Player.hpp"
#include "../GameClass/Enemy.hpp"

void core::Application::run()
{
	LOG_INFO("Running...");

	LOG_INFO("Init Renderer");
	auto& renderer = Renderer::getInstance();
	auto& window = renderer.getWindow();
	float camSpeed = 400.f;
	renderer.setCamSpeed(camSpeed);
	renderer.setCamScope({0.f,1150.f});

	LOG_INFO("Init TextureManager");
	auto& textureManager = ResourceManager<sf::Texture>::getInstance();

	LOG_INFO("Init FontsManager");
	auto& fontManager = ResourceManager<sf::Font>::getInstance();

	LOG_INFO("Init ClipManager");
	_clips = std::make_unique<ClipManager>();

	LOG_INFO("Loading mobinfo json");
	{
		std::ifstream reader("Data/mobInfo.json");
		if (!reader.good())
		{
			LOG_ERROR("Can not open mobinfo json file");
			return;
		}
		reader >> _mobInfoJson;
		reader.close();
	}

	LOG_INFO("Loading cannonInfo json");
	{
		std::ifstream reader("Data/cannonsInfo.json");
		if (!reader.good())
		{
			LOG_ERROR("Can not open cannonInfo json file");
			return;
		}
		reader >> _cannonInfoJson;
		reader.close();
	}

	_clock.restart();
	float frameStartTime;

	assetLoader();
	{
		auto k = core::ResourceManager<sf::Texture>::getInstance().get("Assets/background/1.png");
		if (k == nullptr) {
			LOG_ERROR("Resource manager get(Assets/background/1.png) -> nullptr");
		}
		else {
			std::unique_ptr<BackGround> bcg = std::make_unique<BackGround>();
			bcg->setPosition(sf::Vector2f(-50.f, 0.f));
			bcg->setTexture(*k);
			bcg->setScale(sf::Vector2f(1.f, 1.07f));
			renderer.addObject(std::move(bcg), base::object_type::background);
		}
		k = core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/4.png");
		if (k == nullptr) {
			LOG_ERROR("Resource manager get(Assets/gui/4.png) -> nullptr");
		}
		else {
			std::unique_ptr<BackGround> bcg = std::make_unique<BackGround>();
			bcg = std::make_unique<BackGround>();
			bcg->setTexture(*k);
			bcg->setPosition(sf::Vector2f(200.f, 0.f));
			renderer.addObject(std::move(bcg), base::object_type::gui);
		}	
	}

	std::unique_ptr<Player> player = std::make_unique<Player>();
	player->setPosition(sf::Vector2f(140, 870));
	renderer.addObject(std::move(player), base::object_type::actor);
	
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	enemy->setPosition(sf::Vector2f(2900, 870));
	renderer.addEnemyObject(std::move(enemy));

	float lastFpsTimeCheck = 0.f;
	sf::Text fpsPlayer;
	fpsPlayer.setCharacterSize(20);
	fpsPlayer.setFont(*fontManager.get("Assets/fonts/3.ttf"));
	fpsPlayer.setPosition(sf::Vector2f(10.f, 10.f));
	fpsPlayer.setFillColor(sf::Color::Red);

	fpsPlayer.setString("60");
	LOG_INFO("Main loop...");
	while (window->isOpen())
	{
		frameStartTime = _clock.getElapsedTime().asSeconds();
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				LOG_INFO("Window has been closed");
				return;
			}
		}
		renderer.updateCollision();
		renderer.update();

		auto pos = sf::Mouse::getPosition(*window);
		if (pos.y > window->getSize().y * 0.2f)
		{
			if (pos.x < window->getSize().x * 0.1f)
			{
				renderer.move(sf::Vector2f(camSpeed * _deltaTime, 0));
			}
			else if (pos.x > window->getSize().x * 0.9f)
			{
				renderer.move(sf::Vector2f(-camSpeed * _deltaTime, 0));
			}
		}

		window->clear();
		renderer.draw();

		window->draw(fpsPlayer);

		window->display();
		_deltaTime = _clock.getElapsedTime().asSeconds() - frameStartTime;
		lastFpsTimeCheck += _deltaTime;
		if (lastFpsTimeCheck > 0.2f)
		{
			fpsPlayer.setString(std::to_string(static_cast<int>(1.f / _deltaTime)));
			lastFpsTimeCheck = 0.f;
		}
	}
	return;
}

const base::Clip& core::Application::getClip(const char* name)
{
	return *_clips->getClip(name);
}

void core::Application::assetLoader()
{
	auto &texture = ResourceManager<sf::Texture>::getInstance();
	auto& font = ResourceManager<sf::Font>::getInstance();
	std::string path;
	std::unique_ptr<base::Clip> clip;
	clip = std::make_unique<base::Clip>();
	
	for (int k = 1; k < 44; ++k)
	{
		path = "Assets/mobs/I/caveman/walk/";
		path += std::to_string(k);
		path += ".png";
		texture.loadFromFile(path.c_str());
		clip->addFrame(texture.get(path.c_str()));
	}
	clip->setOrigin(sf::Vector2f(27.f,45.f ));
	_clips->addClip(std::move(clip), "caveman_walk");


	clip = std::make_unique<base::Clip>();
	clip->setOrigin(sf::Vector2f(12.f, 45.f));
	for (int k = 1; k < 51; ++k)
	{
		path = "Assets/mobs/I/caveman/idle/";
		path += std::to_string(k);
		path += ".png";
		texture.loadFromFile(path.c_str());
		clip->addFrame(texture.get(path.c_str()));
	}
	_clips->addClip(std::move(clip), "caveman_idle");


	clip = std::make_unique<base::Clip>();
	clip->setOrigin(sf::Vector2f(72.f, 45.f));
	for (int k = 1; k < 25; ++k)
	{
		path = "Assets/mobs/I/caveman/die/";
		path += std::to_string(k);
		path += ".png";
		texture.loadFromFile(path.c_str());
		clip->addFrame(texture.get(path.c_str()));
	}
	_clips->addClip(std::move(clip), "caveman_die");

	clip = std::make_unique<base::Clip>();
	clip->setOrigin(sf::Vector2f(39.f, 72.f));
	for (int k = 1; k < 41; ++k)
	{
		path = "Assets/mobs/I/caveman/attack/";
		path += std::to_string(k);
		path += ".png";
		texture.loadFromFile(path.c_str());
		clip->addFrame(texture.get(path.c_str()));
	}
	_clips->addClip(std::move(clip), "caveman_attack");

	path = "Assets/background/1.png";
	texture.loadFromFile(path.c_str());


	texture.loadFromFile("Assets/base/1.png");

}

core::Application::Application()
	:_deltaTime(0.f)
{}