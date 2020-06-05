#pragma once
#include "ActorAnimator.hpp"
namespace base {
	class Mob : public ActorAnimator {
	public:
		Mob(int hp = 80, int minattack = 15, int maxattack = 30, float speedattack = 1.f,float speedmove = 5.f, int income = 0);
		Mob(const Mob&) = default;
		Mob(Mob&&) = default;
		Mob& operator=(const Mob&) = default;
		Mob& operator=(Mob&&) = default;
	
		virtual ~Mob() = default;

		virtual void damage(int dmg);

		virtual void setTeam(const team& tm) override;

		virtual void setSpeedMove(const float speed);

		virtual void setSpeedAttack(const float speed);

		virtual void setAttack(const int min, const  int max);

		virtual void setAnimatorName(const std::string& name) = 0;

		virtual bool isColliderActive() const override {
			return m_activeCollider;
		}

	protected:
		int m_hp;
		int m_attack;
		int m_maxAttack;
		float m_speedAttack;
		float m_speedMove;
		bool m_activeCollider;
		int m_income;
	};

}