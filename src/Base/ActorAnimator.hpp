#pragma once
#include <map>
#include <memory>
#include <string>
#include "Actor.hpp"
#include "Clip.hpp"

namespace base {

	class ActorAnimator : public Actor {
	public:

		ActorAnimator();
		ActorAnimator(const ActorAnimator&);
		ActorAnimator(ActorAnimator&&) noexcept;

		ActorAnimator& operator=(const ActorAnimator&);
		ActorAnimator& operator=(ActorAnimator&&) noexcept;

		~ActorAnimator() = default;
		
		virtual bool play(const char* key);

		virtual void addClip(std::shared_ptr<Clip>& clip,const std::string& name);
		
		void updateAnimator();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		virtual void setPosition(const sf::Vector2f& pos);

		virtual void setScale(const sf::Vector2f& sca);

		virtual void move(const sf::Vector2f& delta);

		virtual const std::string& getCurrentClipName() const { return _currentClip->first; }

	protected:

		std::map<std::string, std::shared_ptr<Clip>> _container;
		std::map<std::string, std::shared_ptr<Clip>>::iterator _currentClip;

		std::shared_ptr<sf::Sprite> _sprite;
		bool _isRunning;
		bool _isFinish;

	};

}