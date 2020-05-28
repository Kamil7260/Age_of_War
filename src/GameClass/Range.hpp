#pragma once
#include "../Base/Mob.hpp"

class Range : public base::Mob {
public:
	Range(const base::collider& collider = { 1.f,1.f,1.f,1.f }, int hp = 30, int attack = 10, int maxAttack = 15, int range = 250, float speedAttack = 1.f, float speedMove = 25.f);
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
	float _timer;
	float _attackTimer;
	int _range;

	std::string _attackClip;
	std::string _idleClip;
	std::string _walkClip;
	std::string _dieClip;
	std::string _walkShotClip;
	std::string _idleShotClip;

	bool _touchEnemy;
	bool _isCollided;
	bool _enableAttack;
	bool _died;
	bool _inRange;
};