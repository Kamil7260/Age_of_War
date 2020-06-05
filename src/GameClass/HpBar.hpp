#pragma once
#include "../Base/Actor.hpp"
#include <functional>
class HpBar : public base::Actor {
public:
	HpBar(int* target, int x = 50, int y = 200);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void onUpdate() override;
	virtual void setPosition(const sf::Vector2f& pos) override;
	virtual void move(const sf::Vector2f& delta) override;

	virtual void setFont(const sf::Font& font);
	virtual void setCharacterSize(const unsigned int size);

	virtual void setCallbackOnDie(const std::function<void()>& func);
protected:
	sf::Text m_text;
	sf::RectangleShape m_bar;
	sf::VertexArray m_frame;
	int* m_target;
	int m_x, m_y;
	int m_current;
	std::function<void()> m_callBackOnDie;
	bool m_call;
	int m_maxHP;
};