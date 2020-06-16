#include <cmath>

#include "Cannon.hpp"
#include "../Core/Renderer.hpp"
#include "../Core/Application.hpp"
#include "Bullet.hpp"

Cannon::Cannon(int maxDMG, int minDMG, int range, float reloadTime,float bulletspeed, const float deltaBulletPos, const float fireSpeed,const int sellPrice)
	:m_maxDMG(maxDMG), m_minDMG(minDMG), m_range(range), m_reloadTime(reloadTime), m_curTime(0.f), m_enableShot(true),
	m_bulletSpeed(bulletspeed), m_rangeSpawn(deltaBulletPos), m_longRange(false), m_fireSpeed(fireSpeed), m_sellPrice(sellPrice)
{
	m_team = base::team::player;
}

void Cannon::onUpdate()
{
	updateAnimator();
	auto& k = core::Renderer::getInstance().isEnemyInRange(m_position, m_range, m_team);
	
	if (k != nullptr)
	{
		correctDirection(k);
		if (!m_isRunning)
		{
			m_curTime += core::Application::getInstance().getTime();
			if (m_curTime > m_reloadTime)
			{
				play(m_currentClipName);
				m_curTime = 0.f;
			}
		}
	}
	else {
		m_curTime = 0.f;
	}
}

void Cannon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_sprite, states);
}

void Cannon::setBulletTexture(const std::shared_ptr<sf::Texture>& tex)
{
	m_bulletTex = tex;
}

void Cannon::addClip(base::Clip clip, const std::string& name)
{
	m_sprite->setTexture(*clip.getMask());
	m_sprite->setOrigin(clip.getOriginMask());
	clip.setSprite(m_sprite);
	m_currentClipName = name;
	clip.setCallbackOnTime([&]()->void {
		m_enableShot = true;

		auto r = m_sprite->getRotation();
		auto x = std::cos(r * 3.14f / 180.f);
		auto y = std::sin(r * 3.14f / 180.f);
		if (m_longRange) {
			y += 0.05f;
			x -= 0.05f;
		}
		std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>(m_minDMG,m_maxDMG,m_bulletSpeed, sf::Vector2f(x,y));
		bullet->setRotation(static_cast<int>(m_sprite->getRotation()));
		if (m_longRange)
		{
			r+=100;
			r = 180 - r;
			x = std::cos(r * 3.14f / 180.f) * m_rangeSpawn;
			y = std::sin(r * 3.14f / 180.f) * m_rangeSpawn;
		}
		else {
			x = 0.f;
			y = 0.f;
		}
		m_bulletSpawnPoint = { m_position.x - x, m_position.y - y };
		bullet->setPosition(m_bulletSpawnPoint);
		bullet->setTexture(*m_bulletTex);
		bullet->setTeam(m_team);
		core::Renderer::getInstance().addBullet(std::move(bullet));
		}, m_fireSpeed);
	m_container.insert(std::make_pair(name, std::move(clip)));
}

void Cannon::correctDirection(std::unique_ptr<base::Actor>& target)
{
	auto k = target->getPosition();
	float y = m_position.y - k.y;
	float z = k.x - m_position.x;
	z = std::sqrt(y * y + z * z);
	float val =  std::acos(y/z) * 180.f / 3.14f;
	if (m_longRange)
	{
		auto angle = -(90 - val);
		if(angle>15)
			m_sprite->setRotation(15);
		else {
			m_sprite->setRotation(angle);
		}
		return;
	}
	m_sprite->setRotation(-(90 - val));
}
