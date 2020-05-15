#include "Application.hpp"
#include "ResourceManager.hpp"

void core::Application::run()
{
	LOG_INFO("Running...");

	LOG_INFO("Init Renderer");
	auto& renderer = Renderer::getInstance();
	auto& window = renderer.getWindow();

	LOG_INFO("Init TextureManager");
	auto& textureManager = ResourceManager<sf::Texture>::getInstance();

	_clock.restart();
	float frameStartTime;

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

core::Application::Application()
	:_deltaTime(0.f)
{}