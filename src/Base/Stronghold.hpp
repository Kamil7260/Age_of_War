#pragma once
#include "Actor.hpp"

namespace base {

	class Stronghold : public Actor {
	public:
		Stronghold();
		Stronghold(const Stronghold&) = delete;
		Stronghold(Stronghold&&) = default;
		Stronghold& operator=(const Stronghold&) = delete;
		Stronghold& operator=(Stronghold&&) = default;

		virtual ~Stronghold() = default;

		virtual void damage(int dmg);

		virtual bool isColliderActive() const override { return _activeCollider; }
	protected:
		int _hp;
		bool _activeCollider;

	};

}