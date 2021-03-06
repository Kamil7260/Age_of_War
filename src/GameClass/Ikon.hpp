#pragma once
#include <functional>

#include "../Base/Actor.hpp"

class Ikon: public base::Actor
{
public:
	Ikon(float time = 2.f);
	Ikon(const Ikon&) = default;
	Ikon(Ikon&&) = default;

	Ikon& operator=(const Ikon&) = default;
	Ikon& operator=(Ikon&&) = default;

	virtual ~Ikon() = default;

	virtual void onUpdate() override;
	virtual void setPosition(const sf::Vector2f& pos) override;
	virtual void setScale(const sf::Vector2f& sca) override;
	virtual void move(const sf::Vector2f& delta) override;

	virtual void setTexture(const sf::Texture& tex);

	virtual void setTimePoint(const float points);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void setCallBackOnEnd(const std::function<void()>& onEnd);
protected:
	sf::Sprite m_sprite;
	float m_maxTime;
	float m_timer;
	sf::RectangleShape m_shadow;
	std::function<void()> m_onEnd;
	sf::Vector2f m_shadowSize;
};