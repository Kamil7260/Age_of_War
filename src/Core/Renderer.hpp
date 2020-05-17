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

		void addObject(std::unique_ptr<base::Actor> actor, base::object_type layer);

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
		
}