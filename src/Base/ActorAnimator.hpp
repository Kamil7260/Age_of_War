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

		virtual const std::string& getCurrentClipName() const { return _currentClip->first; }

	protected:

		std::map<std::string, Clip> _container;
		std::map<std::string, Clip>::iterator _currentClip;

		std::shared_ptr<sf::Sprite> _sprite;
		bool _isRunning;
		bool _isFinish;
		float _animationSpeed;

		std::string _currentClipName;
	};

}