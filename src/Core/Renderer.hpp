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
		void addEnemyObject(std::unique_ptr<base::Actor> actor);

		void update();

		void draw();

		void clearNoActive();

		const std::unique_ptr<sf::RenderWindow>& getWindow() {
			return _window;
		};

		std::unique_ptr<base::Actor>& isEnemyInRange(const sf::Vector2f& myPosition, const float myRange, const base::team& myTeam);

		void updateCollision();

		void move(const sf::Vector2f& delta);

		void setCamSpeed(const float speed) { _camSpeed = speed; }
		void setCamScope(const sf::Vector2f& scope) { _scope = scope; }
	private:
		float _camSpeed;
		sf::Vector2f _position;
		sf::Vector2f _scope;
		std::unique_ptr<base::Actor>& getLastColliderActor(std::vector<std::unique_ptr<base::Actor>>& actor);

		void eraseNoActive();

		void insertQueue();

		void collisionBetween(std::unique_ptr<base::Actor>& left, std::unique_ptr<base::Actor>& right) const ;

		Renderer();

		std::vector<std::pair<std::unique_ptr<base::Actor>,base::object_type>> _queue;
		std::vector<std::unique_ptr<base::Actor>> _enemyQueue;

		std::vector<std::unique_ptr<base::Actor>> _backGround;
		std::vector<std::unique_ptr<base::Actor>> _actor;
		std::vector<std::unique_ptr<base::Actor>> _enemyActor;
		std::vector<std::unique_ptr<base::Actor>> _gui;
		std::unique_ptr<sf::RenderWindow> _window;

		std::unique_ptr<base::Actor> _placeHolder;
		bool _shouldBeErase;
	};
		
}