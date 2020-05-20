#pragma once
#include "ActorAnimator.hpp"
namespace base {
	class Mob : public ActorAnimator {
	public:
		Mob(int hp = 100, int attack = 10, float speedattack = 5.f,float speedmove = 5.f);
		Mob(const Mob&);
		Mob(Mob&&) noexcept;
		Mob& operator=(const Mob&);
		Mob& operator=(Mob&&) noexcept;
	
		virtual void damage(int dmg);

		virtual void setTeam(const team& tm) override;

		virtual void setAnimatorName(const char* name) = 0;
	protected:
		int _hp;
		int _attack;
		float _speedAttack;
		float _speedMove;
	};

}