#pragma once
#include "Info.hpp"
#include <functional>
class Button : public base::Actor
{
public:
	Button(const base::collider& collider = {1.f,1.f,1.f,1.f});
	Button(const Button&) = default;
	Button(Button&&) = default;

	Button& operator=(const Button&) = default;
	Button& operator=(Button&&) = default;

	virtual ~Button() = default;
	virtual void setTexture(const sf::Texture& tex);
	virtual void onUpdate() override;
	virtual void onMouseCollision(bool isPressed) override;
	virtual void setPosition(const sf::Vector2f& pos) override;
	virtual void setScale(const sf::Vector2f& sca) override;
	virtual void move(const sf::Vector2f& delta) override;

	virtual Info& getInfo();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void setClickEvent(const std::function<void(bool)>& func);
protected:
	sf::Sprite _sprite;
	std::function<void(bool)> _onClickEvent;
	sf::RectangleShape _shadow;
	bool _shadowDraw;
	Info _infoBox;
	bool _drawBox;
	bool _isCollided;
};