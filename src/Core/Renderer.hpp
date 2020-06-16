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
			return m_window;
		};

		std::unique_ptr<base::Actor>& isEnemyInRange(const sf::Vector2f& myPosition, const int myRange, const base::team& myTeam);

		void updateCollision();

		void move(const sf::Vector2f& delta);

		std::unique_ptr<base::Actor>& find(const std::string& tag);
		std::unique_ptr<base::Actor> findAndRemove(const std::string& tag);

		const sf::Vector2i getMousePosition() {
			return sf::Mouse::getPosition(*m_window);
		}

		void setCamSpeed(const float speed) { m_camSpeed = speed; }
		void setCamScope(const sf::Vector2f& scope) { m_scope = scope; }

		void forceClear();

		sf::View& getView() {
			return m_view;
		}

		const size_t getPlayerUnitsCount() const {
			return m_actor.size();
		}

		const sf::Vector2f& getPlayerLeaderPosition() const {
			return m_actor.back()->getPosition();
		}
		
		void resetPosition(const sf::Vector2f& correct) {
			m_position = correct;
		}

	private:
		float m_camSpeed;
		sf::Vector2f m_position;
		sf::Vector2f m_scope;
		std::unique_ptr<base::Actor>& getLastColliderActor(std::vector<std::unique_ptr<base::Actor>>& actor);

		void eraseNoActive();

		void insertQueue();

		bool collisionBetween(std::unique_ptr<base::Actor>& left, std::unique_ptr<base::Actor>& right) const;
		bool collisionBetweenBack(std::unique_ptr<base::Actor>& left, std::unique_ptr<base::Actor>& right) const;
		std::vector<std::unique_ptr<base::Actor>>::iterator& getNextActiveCollider(std::vector<std::unique_ptr<base::Actor>>::iterator& current, const std::vector<std::unique_ptr<base::Actor>>::iterator& last);

		void onMouse(const std::unique_ptr<base::Actor>& source) const;


		Renderer();

		std::vector<std::pair<std::unique_ptr<base::Actor>,base::object_type>> m_queue;
		std::vector<std::unique_ptr<base::Actor>> m_enemyQueue;
		std::vector<std::unique_ptr<base::Actor>> m_bulletQueue;

		std::vector<std::unique_ptr<base::Actor>> m_backGround;
		std::vector<std::unique_ptr<base::Actor>> m_actor;
		std::vector<std::unique_ptr<base::Actor>> m_bulletActor;

		std::vector<std::unique_ptr<base::Actor>> m_enemyBulletActor;
		std::vector<std::unique_ptr<base::Actor>> m_enemyActor;
		std::vector<std::unique_ptr<base::Actor>> m_gui;
		std::unique_ptr<sf::RenderWindow> m_window;

		std::unique_ptr<base::Actor> m_placeHolder;
		bool m_shouldBeErase;
		sf::Vector2i m_mousePosition;
		sf::View m_view;
	};
		
}