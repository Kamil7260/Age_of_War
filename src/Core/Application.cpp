#include <string>
#include "Application.hpp"
#include "ResourceManager.hpp"

#include "../GameClass/Background.hpp"
#include "../GameClass/Melee.hpp"
#include "../GameClass/Player.hpp"

void core::Application::run()
{
	LOG_INFO("Running...");

	LOG_INFO("Init Renderer");
	auto& renderer = Renderer::getInstance();
	auto& window = renderer.getWindow();

	LOG_INFO("Init TextureManager");
	auto& textureManager = ResourceManager<sf::Texture>::getInstance();

	LOG_INFO("Init ClipManager");
	_clips = std::make_unique<ClipManager>();

	_clock.restart();
	float frameStartTime;

	assetLoader();
	
	std::unique_ptr<BackGround> bcg = std::make_unique<BackGround>();
	bcg->setPosition(sf::Vector2f(-50.f, 0.f));

	renderer.addObject(std::move(bcg), base::object_type::background);

	std::unique_ptr<Player> player = std::make_unique<Player>();
	player->setPosition(sf::Vector2f(-180, 600));
	renderer.addObject(std::move(player), base::object_type::actor);
	
	std::unique_ptr<Melee> man = std::make_unique<Melee>();
	man->setAnimatorName("caveman");
	man->setPosition(sf::Vector2f(830, 850));
	core::Renderer::getInstance().addEnemyObject(std::move(man));


	//std::unique_ptr<Caveman> man = std::make_unique<Caveman>();
	//man->setPosition(sf::Vector2f(100.f, 100.f));

	//renderer.addObject(std::move(man), base::object_type::actor);

	//man = std::make_unique<Caveman>();
	//man->setPosition(sf::Vector2f(200.f, 100.f));

	//renderer.addObject(std::move(man), base::object_type::actor);

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
		window->clear();
		renderer.draw();
		window->display();
		_deltaTime = _clock.getElapsedTime().asSeconds() - frameStartTime;
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