#pragma once
#include "../Base/Actor.hpp"
#include "../Logger/Logger.hpp"
#include <memory>
#include <functional>

namespace core {

	class Renderer final {
	public:
		Renderer(Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;

		~Renderer() = default;
		
		Renderer& getInstance() {
			static Renderer instance;
			return instance;
		}

		template<typename T>
		void addObject(base::object_type layer, base::team team = base::team::none, const std::function<void(const std::unique_ptr<T>& ptr)>& call = nullptr);

		void update();

		void draw();

		void clear_no_active();

	private:
		Renderer();

		std::vector<std::unique_ptr<base::Actor>> _backGround;
		std::vector<std::unique_ptr<base::Actor>> _actor;
		std::vector<std::unique_ptr<base::Actor>> _gui;

		std::unique_ptr<sf::RenderWindow> _window;

	};


	template<typename T>
	void Renderer::addObject(base::object_type layer, base::team team, const std::function<void(const std::unique_ptr<T>& ptr)>& call)
	{
		switch (layer)
		{
		case base::object_type::actor:
			std::unique_ptr<base::Actor> ptr = std::make_unique<T>();
			ptr.back->setType(layer);
			ptr.back->setTeam(team);
			if(ptr != nullptr)
				call(ptr);
			_actor.push_back(ptr.release());
			break;
		case base::object_type::background:
			std::unique_ptr<base::Actor> ptr = std::make_unique<T>();
			ptr.back->setType(layer);
			ptr.back->setTeam(team);
			if (ptr != nullptr)
				call(ptr);
			_backGround.push_back(ptr.release());
			break;
		case base::object_type::gui:
			std::unique_ptr<base::Actor> ptr = std::make_unique<T>();
			ptr.back->setType(layer);
			ptr.back->setTeam(team);
			if (ptr != nullptr)
				call(ptr);
			_gui.push_back(ptr.release());
			break;
		}
	}

}