#pragma once

#include "../Base/ActorAnimator.hpp"

class Counter : public base::ActorAnimator
{
public:
	Counter(int* target);
	Counter(const Counter&) = default;
	Counter(Counter&&) = default;

	Counter& operator=(const Counter&) = default;
	Counter& operator=(Counter&&) = default;
	~Counter() = default;

	void setFont(const sf::Font& font);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setScale(const sf::Vector2f& sca) override;

	void move(const sf::Vector2f& delta) override;

	void setPosition(const sf::Vector2f& pos) override;

	void setColor(const sf::Color& color);

	void onUpdate() override;

	virtual void addClip(base::Clip clip, const std::string& name) override;

	void setCharacterSize(const unsigned int size);
protected:
	sf::Text m_text;
	int* m_target;

	float m_curTime;
	int m_currentValue;
};