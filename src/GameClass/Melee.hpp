#pragma once
#include "../Base/Mob.hpp"

class Melee : public base::Mob {
public:
	Melee(const base::collider& collider = { 1.f,1.f,1.f,1.f }, int hp = 100, int attack = 10, float speedAttack = 4.f, float speedMove = 5.f);
	Melee(Melee&);
	Melee(Melee&&) noexcept;

	Melee& operator=(Melee&);
	Melee& operator=(Melee&&) noexcept;

	void setAnimatorName(const char* name) override;

	void onUpdate();

	void setPosition(const sf::Vector2f& pos) override;

	void onCollision(std::unique_ptr<base::Actor>& collision) override;

private:
	float _timer;

	std::string _attackClip;
	std::string _idleClip;
	std::string _walkClip;
	std::string _dieClip;

	bool _touchEnemy;
	bool _isCollided;
};