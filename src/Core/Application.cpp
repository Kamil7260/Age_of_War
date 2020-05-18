#include <string>
#include "Application.hpp"
#include "ResourceManager.hpp"

#include "../GameClass/Background.hpp"


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
	bcg->setPosition(sf::Vector2f(0.f, 0.f));

	renderer.addObject(std::move(bcg), base::object_type::background);

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
		renderer.update();
		window->clear();
		renderer.draw();
		window->display();
		_deltaTime = _clock.getElapsedTime().asSeconds() - frameStartTime;
	}
	return;
}

const std::shared_ptr<base::Clip> core::Application::getClip(const char* name)
{
	return _clips->getClip(name);
}

void core::Application::assetLoader()
{
	auto &texture = ResourceManager<sf::Texture>::getInstance();
	std::string path;
	for (int k = 1; k < 44; ++k)
	{
		path = "Assets/mobs/I/caveman/walk/";
		path += std::to_string(k);
		path += ".png";
		texture.loadFromFile(path.c_str());
	}
	for (int k = 1; k < 51; ++k)
	{
		path = "Assets/mobs/I/caveman/idle/";
		path += std::to_string(k);
		path += ".png";
		texture.loadFromFile(path.c_str());
	}
	for (int k = 1; k < 25; ++k)
	{
		path = "Assets/mobs/I/caveman/die/";
		path += std::to_string(k);
		path += ".png";
		texture.loadFromFile(path.c_str());
	}
	for (int k = 1; k < 41; ++k)
	{
		path = "Assets/mobs/I/caveman/attack/";
		path += std::to_string(k);
		path += ".png";
		texture.loadFromFile(path.c_str());
	}
	path = "Assets/background/1.png";
	texture.loadFromFile(path.c_str());
}

core::Application::Application()
	:_deltaTime(0.f)
{}