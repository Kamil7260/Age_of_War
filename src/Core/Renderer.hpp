#pragma once
#include <memory>
#include <functional>
#include "../Base/ActorAnimator.hpp"
#include "../Logger/Logger.hpp"

namespace core {

	class Renderer final {
	public:
		Renderer(Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;

		~Renderer() = default;
		
		static Renderer& getInstance() {
			static Renderer instance;
			return instance;
		}

		template<typename T>
		void addObject(base::object_type layer, base::team team = base::team::none);

		void update();

		void draw();

		void clearNoActive();

		const std::unique_ptr<sf::RenderWindow>& getWindow() {
			return _window;
		};

	private:
		Renderer();

		std::vector<std::unique_ptr<base::Actor>> _backGround;
		std::vector<std::unique_ptr<base::Actor>> _actor;
		std::vector<std::unique_ptr<base::Actor>> _gui;
		std::unique_ptr<sf::RenderWindow> _window;
	};

	template<typename T>
	void Renderer::addObject(base::object_type layer, base::team team)
	{
		clearNoActive();
		switch (layer)
		{
		case base::object_type::actor:
			std::unique_ptr<base::Actor> ptr = std::make_unique<T>();
			ptr->setType(layer);
			ptr->setTeam(team);
			ptr->onStart();
			_actor.push_back(ptr.release());
			break;
		case base::object_type::background:
			std::unique_ptr<base::Actor> ptr = std::make_unique<T>();
			ptr.back->setType(layer);
			ptr.back->setTeam(team);
			ptr->onStart();
			_backGround.push_back(ptr.release());
			break;
		case base::object_type::gui:
			std::unique_ptr<base::Actor> ptr = std::make_unique<T>();
			ptr.back->setType(layer);
			ptr.back->setTeam(team);
			ptr->onStart();
			_gui.push_back(ptr.release());
			break;
		}
	}
}