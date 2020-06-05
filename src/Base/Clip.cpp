#include "Clip.hpp"
#include "../Core/Application.hpp"

base::Clip::Clip(float speed)
	: m_isFinish(true), m_speed(speed), m_curTime(0.f), m_origin(0.5f, 0.5f), m_callback(nullptr), m_callbackOnTime(nullptr),
	m_onTime(0.f), m_onTimeCalled(false), m_onTimeCounter(0.f)
{
}

void base::Clip::addFrame(const std::shared_ptr<sf::Texture>& tex)
{
	m_container.push_back(tex);
}

bool base::Clip::update()
{
	auto delta = core::Application::getInstance().getTime();
	m_curTime += delta;
	 
	if (m_onTimeCounter > 0.f)
	{
		m_onTimeCounter -= delta;
	}
	else if(!m_onTimeCalled) {
		if (m_callbackOnTime != nullptr) {
			m_callbackOnTime();
			m_onTimeCalled = true;
		}
	}
	if (m_curTime >= m_speed)
	{
		m_curTime = 0.f;
		++m_currentFrame;
		if (m_currentFrame == m_container.end())
		{
			m_currentFrame = m_container.begin();
			m_isFinish = true;
			if (m_callback != nullptr)
				m_callback();
			return true;
		}
		m_sprite->setTexture(*(*m_currentFrame));
	}
	return false;
}

void base::Clip::setOrigin(const sf::Vector2f& origin)
{
	m_origin = origin;
}

void base::Clip::setSprite(const std::shared_ptr<sf::Sprite>& sprite)
{
	m_sprite = sprite;
}

void base::Clip::setSpeed(const float speed)
{
	m_speed = speed;
}

void base::Clip::start()
{
	m_onTimeCalled = false;
	m_isFinish = false;
	m_currentFrame = m_container.begin();
	m_sprite->setTexture(*(*m_currentFrame),true);
	m_curTime = 0.f;
	m_onTimeCounter = m_onTime;
	m_sprite->setOrigin(m_origin);
}

const sf::Vector2f& base::Clip::getOriginMask() const
{
	return m_origin;
}

void base::Clip::setCallbackOnTime(const std::function<void()>& callme, const float time)
{
	m_callbackOnTime = callme;
	m_onTime = time;
}

std::shared_ptr<sf::Texture> base::Clip::getMask() const
{
	if (m_container.empty())
		return nullptr;
	return *m_container.begin();
}

void base::Clip::setCallback(const std::function<void()>& callme)
{
	m_callback = callme;
}
