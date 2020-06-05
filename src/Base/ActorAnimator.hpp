#pragma once
#include <map>
#include <memory>
#include <string>
#include "Actor.hpp"
#include "Clip.hpp"

namespace base {

	class ActorAnimator : public Actor {
	public:

		ActorAnimator(float animationspeed = 0.2f);
		ActorAnimator(const ActorAnimator&) = default;
		ActorAnimator(ActorAnimator&&) = default;

		ActorAnimator& operator=(const ActorAnimator&) = default;
		ActorAnimator& operator=(ActorAnimator&&) = default;

		virtual ~ActorAnimator() = default;
		
		virtual bool play(const std::string& key);

		virtual void addClip(Clip clip,const std::string& name);
		
		virtual void updateAnimator();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		virtual void setPosition(const sf::Vector2f& pos);

		virtual void setScale(const sf::Vector2f& sca);

		virtual void move(const sf::Vector2f& delta);

		virtual void setAnimationSpeed(const float speed); 

		virtual const std::string& getCurrentClipName() const { return m_currentClip->first; }

	protected:

		std::map<std::string, Clip> m_container;
		std::map<std::string, Clip>::iterator m_currentClip;

		std::shared_ptr<sf::Sprite> m_sprite;
		bool m_isRunning;
		bool m_isFinish;
		float m_animationSpeed;

		std::string m_currentClipName;
	};

}