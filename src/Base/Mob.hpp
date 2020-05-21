#pragma once
#include "ActorAnimator.hpp"
namespace base {
	class Mob : public ActorAnimator {
	public:
		Mob(int hp = 80, int attack = 20, float speedattack = 1.f,float speedmove = 5.f);
		Mob(const Mob&);
		Mob(Mob&&) noexcept;
		Mob& operator=(const Mob&);
		Mob& operator=(Mob&&) noexcept;
	
		virtual void damage(int dmg);

		virtual void setTeam(const team& tm) override;

		virtual void setSpeedMove(const float speed);

		virtual void setSpeedAttack(const float speed);

		virtual void setAnimatorName(const char* name) = 0;

		virtual bool isColliderActive() const override {
			return _activeCollider;
		}

	protected:
		int _hp;
		int _attack;
		float _speedAttack;
		float _speedMove;
		bool _activeCollider;
	};

}