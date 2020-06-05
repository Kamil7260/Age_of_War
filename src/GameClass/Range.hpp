#pragma once
#include "../Base/Mob.hpp"

class Range : public base::Mob {
public:
	Range(const base::collider& collider = { 1.f,1.f,1.f,1.f }, int hp = 30, int attack = 10, int maxAttack = 15, int range = 250, float speedAttack = 1.f, float speedMove = 25.f, int income = 0);
	Range(const Range&) = default;
	Range(Range&&) = default;

	Range& operator=(const Range&) = default;
	Range& operator=(Range&&) = default;

	virtual ~Range() = default;

	virtual void setAnimatorName(const std::string& name) override;

	virtual void onUpdate() override;

	virtual void damage(int dmg) override;

	virtual void setPosition(const sf::Vector2f& pos) override;

	virtual	void onCollision(std::unique_ptr<base::Actor>& collision) override;

private:
	float m_timer;
	float m_attackTimer;
	int m_range;

	std::string m_attackClip;
	std::string m_idleClip;
	std::string m_walkClip;
	std::string m_dieClip;
	std::string m_walkShotClip;
	std::string m_idleShotClip;

	bool m_touchEnemy;
	bool m_isCollided;
	bool m_enableAttack;
	bool m_died;
	bool m_inRange;
};