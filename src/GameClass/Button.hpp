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

	virtual void setCallbackOnMouseRelease(const std::function<void()>& func);
	virtual void setClickEvent(const std::function<void(bool)>& func);
protected:
	sf::Sprite m_sprite;
	std::function<void(bool)> m_onClickEvent;
	std::function<void()> m_onReleaseEvent;
	bool m_isReleaseCalled;

	sf::RectangleShape m_shadow;
	bool m_shadowDraw;
	Info m_infoBox;
	bool m_drawBox;
	bool m_isCollided;
};