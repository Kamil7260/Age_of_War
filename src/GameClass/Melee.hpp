#pragma once
#include "../Base/Mob.hpp"

class Melee : public base::Mob {
public:
	Melee(const base::collider& collider = { 1.f,1.f,1.f,1.f }, int hp = 50, int attack = 20,int maxAttack = 25, float speedAttack = 1.f, float speedMove = 5.f, int income = 0);
	Melee(const Melee&) = default;
	Melee(Melee&&) = default;

	Melee& operator=(const Melee&) = default;
	Melee& operator=(Melee&&) = default;

	virtual ~Melee() = default;

	void setAnimatorName(const std::string& name) override;

	void onUpdate() override;

	virtual void damage(int dmg) override;

	void setPosition(const sf::Vector2f& pos) override;

	void onCollision(std::unique_ptr<base::Actor>& collision) override;

private:
	float m_timer;
	float m_attackTimer;

	std::string m_attackClip;
	std::string m_idleClip;
	std::string m_walkClip;
	std::string m_dieClip;

	bool m_touchEnemy;
	bool m_isCollided;
	bool m_enableAttack;
	bool m_died;
};