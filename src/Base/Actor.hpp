#pragma once
#include <SFML/Graphics.hpp>
#include <string>
namespace base {

	enum class object_type {
		background,
		actor,
		gui
	};

	enum class team {
		player,
		enemy,
		none
	};

	struct collider {
		float up;
		float down;
		float left;
		float right;
	};


	class Actor : public sf::Drawable {

	public:
		Actor(const base::collider& box = { 1.f,1.f,1.f,1.f });
		Actor(const Actor&) = default;
		Actor(Actor&&) = default;

		virtual ~Actor() = default;

		Actor& operator=(const Actor&) = default;
		Actor& operator=(Actor&&) = default;

		virtual void setPosition(const sf::Vector2f& pos) { m_position = pos; }
		virtual void setScale(const sf::Vector2f& sca) { m_scale = sca; }

		virtual void setType(const object_type& type)  { m_type = type; }
		virtual void setTeam(const team& tm)  { m_team = tm; }

		virtual const sf::Vector2f& getPosition() const { return m_position; }
		virtual const sf::Vector2f& getScale() const { return m_scale; }
		virtual const object_type& getType() const final { return m_type; }
		virtual const team& getTeam() const  final { return m_team; }

		virtual void move(const sf::Vector2f& delta) { m_position += delta; }

		virtual void onCollision(std::unique_ptr<Actor>& collision) {};
		virtual void onMouseCollision(bool isPressed)  {};
		virtual const collider& getCollider() const final { return m_myColider; }
		virtual void setCollider(const collider& source) { m_myColider = source; }

		virtual bool isColliderActive() const { return false; }

		virtual void onStart() {};
		virtual void onUpdate() = 0;

		virtual std::string getTag() const final { return m_tag; }
		virtual void setTag(const std::string& tag) { m_tag = tag; }

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

		virtual bool isActive() const final { return m_isActive; }
		virtual void remove() final { m_isActive = false; }

		virtual void setRotation(const int rot) {
			m_rotation = rot;
		}

		virtual void dynamicDraw(bool ty) {
			m_dynamic = ty;
		}

		virtual bool isDynamic() const {
			return m_dynamic;
		}
	protected:
		std::string m_tag;
		bool m_isActive;
		object_type m_type;
		team m_team;
		sf::Vector2f m_position;
		sf::Vector2f m_scale;
		collider m_myColider;
		int m_rotation;
		bool m_dynamic;
	};
}