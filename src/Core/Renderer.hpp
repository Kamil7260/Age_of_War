#pragma once
#include <memory>
#include <functional>
#include "../Base/ActorAnimator.hpp"
#include "../Logger/Logger.hpp"

namespace core {

	class Renderer final {
	public:
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;

		~Renderer() = default;
		
		static Renderer& getInstance() {
			static Renderer instance;
			return instance;
		}

		void addObject(std::unique_ptr<base::Actor> actor, base::object_type layer);
		void forceAddObject(std::unique_ptr<base::Actor> actor, base::object_type layer);
		void addEnemyObject(std::unique_ptr<base::Actor> actor);
		void addBullet(std::unique_ptr<base::Actor> actor);

		void update();

		void draw();

		void clearNoActive();

		std::unique_ptr<sf::RenderWindow>& getWindow() {
			return _window;
		};

		std::unique_ptr<base::Actor>& isEnemyInRange(const sf::Vector2f& myPosition, const float myRange, const base::team& myTeam);

		void updateCollision();

		void move(const sf::Vector2f& delta);

		std::unique_ptr<base::Actor>& find(const std::string& tag);
		std::unique_ptr<base::Actor> findAndRemove(const std::string& tag);

		const sf::Vector2i getMousePosition() {
			return sf::Mouse::getPosition(*_window);
		}

		void setCamSpeed(const float speed) { _camSpeed = speed; }
		void setCamScope(const sf::Vector2f& scope) { _scope = scope; }

		void forceClear();

		sf::View& getView() {
			return _view;
		}

	private:
		float _camSpeed;
		sf::Vector2f _position;
		sf::Vector2f _scope;
		std::unique_ptr<base::Actor>& getLastColliderActor(std::vector<std::unique_ptr<base::Actor>>& actor);

		void eraseNoActive();

		void insertQueue();

		bool collisionBetween(std::unique_ptr<base::Actor>& left, std::unique_ptr<base::Actor>& right) const ;
		std::vector<std::unique_ptr<base::Actor>>::iterator& getNextActiveCollider(std::vector<std::unique_ptr<base::Actor>>::iterator& current, const std::vector<std::unique_ptr<base::Actor>>::iterator& last);

		void onMouse(const std::unique_ptr<base::Actor>& source) const;


		Renderer();

		std::vector<std::pair<std::unique_ptr<base::Actor>,base::object_type>> _queue;
		std::vector<std::unique_ptr<base::Actor>> _enemyQueue;
		std::vector<std::unique_ptr<base::Actor>> _bulletQueue;

		std::vector<std::unique_ptr<base::Actor>> _backGround;
		std::vector<std::unique_ptr<base::Actor>> _actor;
		std::vector<std::unique_ptr<base::Actor>> _bulletActor;

		std::vector<std::unique_ptr<base::Actor>> _enemyBulletActor;
		std::vector<std::unique_ptr<base::Actor>> _enemyActor;
		std::vector<std::unique_ptr<base::Actor>> _gui;
		std::unique_ptr<sf::RenderWindow> _window;

		std::unique_ptr<base::Actor> _placeHolder;
		bool _shouldBeErase;
		sf::Vector2i _mousePosition;
		sf::View _view;
	};
		
}