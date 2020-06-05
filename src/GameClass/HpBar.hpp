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
	sf::Text _text;
	sf::RectangleShape _bar;
	sf::VertexArray _frame;
	int* _target;
	int _x, _y;
	int _current;
	std::function<void()> _callBackOnDie;
	bool _call;
	int _maxHP;
};