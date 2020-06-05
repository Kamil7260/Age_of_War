#include "ActorAnimator.hpp"

#include "../Logger/Logger.hpp"

base::ActorAnimator::ActorAnimator(float animationspeed)
	:m_isRunning(false),m_isFinish(true), m_animationSpeed(animationspeed)
{
	m_sprite = std::make_shared<sf::Sprite>();
}

bool base::ActorAnimator::play(const std::string& key)
{
	m_currentClip = m_container.find(key);
	if (m_currentClip == m_container.end())
	{
		LOG_ERROR("Can not find Clip with key : ", key);
		m_isRunning = false;
		return m_isRunning;
	}
	m_currentClipName = key;
	m_currentClip->second.setSpeed(m_animationSpeed);
	m_currentClip->second.start();
	m_isRunning = true;
	return m_isRunning;
}

void base::ActorAnimator::addClip(base::Clip clip, const std::string& name)
{
	clip.setSprite(m_sprite);
	m_container.insert(std::make_pair(name, std::move(clip)));
}

void base::ActorAnimator::updateAnimator()
{
	if (m_isRunning)
	{
		m_isFinish = m_currentClip->second.update();
		if (m_isFinish)
			m_isRunning = false;
	}
}

void base::ActorAnimator::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(isActive())
	target.draw(*m_sprite, states);
}

void base::ActorAnimator::setPosition(const sf::Vector2f& pos)
{
	m_position = pos;
	m_sprite->setPosition(pos);
}

void base::ActorAnimator::setScale(const sf::Vector2f& sca)
{
	m_scale = sca;
	m_sprite->setScale(sca);
}

void base::ActorAnimator::move(const sf::Vector2f& delta)
{
	m_position += delta;
	m_sprite->move(delta);
}

void base::ActorAnimator::setAnimationSpeed(const float speed)
{
	m_animationSpeed = speed;
}
