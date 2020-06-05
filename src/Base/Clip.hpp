#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <memory>
namespace base {

	class Clip {
	public:
		Clip(float speed = 1.f);
		Clip(const Clip&) = default;
		Clip(Clip&&) = default;
		Clip& operator=(Clip&) = default;
		Clip& operator=(Clip&&) = default;

		virtual ~Clip() = default;

		virtual void addFrame(const std::shared_ptr<sf::Texture>& tex);
		virtual bool update();

		virtual bool isFinish() const { return m_isFinish; };

		virtual void setOrigin(const sf::Vector2f& origin);

		virtual void setSprite(const std::shared_ptr<sf::Sprite>& sprite);

		virtual void setSpeed(const float speed);

		virtual void start();

		virtual const sf::Vector2f& getOriginMask() const;

		virtual void setCallbackOnTime(const std::function<void()>& callme, const float time);

		virtual std::shared_ptr<sf::Texture> getMask() const;

		virtual void setCallback(const std::function<void()>& callme);
	protected:
		bool m_isFinish;
		std::vector<std::shared_ptr<sf::Texture>> m_container;
		std::vector < std::shared_ptr<sf::Texture>>::iterator m_currentFrame;
		std::shared_ptr<sf::Sprite> m_sprite;
		float m_speed;
		float m_curTime;
		sf::Vector2f m_origin;
		std::function<void()> m_callback;
		std::function<void()> m_callbackOnTime;
		float m_onTime;
		float m_onTimeCounter;
		bool m_onTimeCalled;
	};
}