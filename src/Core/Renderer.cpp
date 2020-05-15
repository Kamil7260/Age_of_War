#include "Renderer.hpp"

void core::Renderer::update()
{
	for (auto k = _actor.begin(); k != _actor.end(); ++k)
	{
		(*k)->onUpdate();
	}
	for (auto k = _gui.begin(); k != _gui.end(); ++k)
	{
		(*k)->onUpdate();
	}
}

void core::Renderer::draw()
{
	for (auto k = _backGround.begin(); k != _backGround.end(); ++k)
	{
		_window->draw((*(*k)));
	}
	for (auto k = _actor.begin(); k != _actor.end(); ++k)
	{
		_window->draw((*(*k)));
	}
	for (auto k = _gui.begin(); k != _gui.end(); ++k)
	{
		_window->draw((*(*k)));
	}
}

void core::Renderer::clearNoActive()
{
	auto fun = [](const std::unique_ptr<base::Actor>& ptr)-> bool {
		return !ptr->isActive();
	};

	auto it1 = std::remove_if(_backGround.begin(), _backGround.end(), fun);
	_backGround.erase(it1, _backGround.end());

	auto it2 = std::remove_if(_actor.begin(), _actor.end(), fun);
	_actor.erase(it2, _actor.end());

	auto it3 = std::remove_if(_gui.begin(), _gui.end(), fun);
	_gui.erase(it3, _gui.end());
}

core::Renderer::Renderer()
{
	LOG_INFO("Creating window...");
	_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1000, 600), "Age of War");
	_window->setFramerateLimit(60);
}
