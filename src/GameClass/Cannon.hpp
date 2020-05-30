#pragma once
#include "../Base/ActorAnimator.hpp"

class Cannon : public base::ActorAnimator {
public:
	Cannon(int maxDMG = 5, int minDMG = 10, int range = 100, float reloadTime = 3.f, float bulletspeed = 400.f, const float deltaBulletPos = 0, const float fireSpeed = 2.f);
	Cannon(const Cannon& source) = default;
	Cannon(Cannon&& source) = default;

	Cannon& operator=(const Cannon& source) = default;
	Cannon& operator=(Cannon&& source) = default;

	virtual ~Cannon() = default;

	virtual void onUpdate() override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void setBulletTexture(const std::shared_ptr<sf::Texture>& tex);

	virtual void addClip(base::Clip clip, const std::string& name);

	virtual void correctDirection(std::unique_ptr<base::Actor>& target);

	virtual void longRange() {
		_longRange = true;
	};
protected:
	int _maxDMG;
	int _minDMG;
	int _range;
	bool _enableShot;
	std::shared_ptr<sf::Texture> _bulletTex;
	float _reloadTime;
	float _curTime;
	float _bulletSpeed;
	sf::Vector2f _bulletSpawnPoint;
	float _rangeSpawn;
	bool _longRange;
	float _fireSpeed;
};