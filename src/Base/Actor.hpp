#pragma once
#include <SFML/Graphics.hpp>
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

	class Actor : public sf::Drawable {

	public:
		Actor() = default;
		Actor(const Actor&) = default;
		Actor(Actor&&) = default;

		virtual ~Actor() = default;

		Actor& operator=(const Actor&) = default;
		Actor& operator=(Actor&&) = default;

		virtual void setPosition(const sf::Vector2f& pos) { _position = pos; }
		virtual void setScale(const sf::Vector2f& sca) { _scale = sca; }

		virtual void setType(const object_type& type) final { _type = type; }
		virtual void setTeam(const team& tm) final { _team = tm; }

		virtual const sf::Vector2f& getPosition() const { return _position; }
		virtual const sf::Vector2f& getScale() const { return _scale; }
		virtual const object_type& getType() const final { return _type; }
		virtual const team& getTeam() const  final { return _team; }

		virtual void move(const sf::Vector2f& delta) { _position += delta; }

		virtual void onCollision(std::unique_ptr<Actor>& collision) {};
		virtual void onStart() { _isActive = true; };
		virtual void onUpdate() = 0;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

		virtual bool isActive() const final { return _isActive; }
		virtual void remove() final { _isActive = false; }

	protected:
		bool _isActive;
		object_type _type;
		team _team;
		sf::Vector2f _position;
		sf::Vector2f _scale;
	};
}