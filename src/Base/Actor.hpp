#pragma once
#include <SFML/Graphics.hpp>
namespace base {

	enum class object_type {
		background,
		actor,
		gui
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

		virtual const sf::Vector2f& getPosition() const { return _position; }
		virtual const sf::Vector2f& getScale() const { return _scale; }

		virtual void move(const sf::Vector2f& delta) { _position += delta; }

		virtual void onUpdate() = 0;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

	protected:

		sf::Vector2f _position;
		sf::Vector2f _scale;
		
	};
}
