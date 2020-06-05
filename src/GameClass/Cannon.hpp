#pragma once
#include "../Base/ActorAnimator.hpp"

class Cannon : public base::ActorAnimator {
public:
	Cannon(int maxDMG = 5, int minDMG = 10, int range = 100, float reloadTime = 3.f, float bulletspeed = 400.f, const float deltaBulletPos = 0, const float fireSpeed = 2.f, const int sellPrice = 20);
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
		m_longRange = true;
	};

	virtual int getSellPrice() const {
		return m_sellPrice;
	}

protected:
	int m_maxDMG;
	int m_minDMG;
	int m_range;
	bool m_enableShot;
	std::shared_ptr<sf::Texture> m_bulletTex;
	float m_reloadTime;
	float m_curTime;
	float m_bulletSpeed;
	sf::Vector2f m_bulletSpawnPoint;
	float m_rangeSpawn;
	bool m_longRange;
	float m_fireSpeed;
	int m_sellPrice;
};