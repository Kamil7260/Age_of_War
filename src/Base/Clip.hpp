#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <memory>
namespace base {

	class Clip {
	public:
		Clip(float speed = 1.f);
		Clip(const Clip&);
		Clip(Clip&&) noexcept;
		Clip& operator=(Clip&);
		Clip& operator=(Clip&&) noexcept;

		~Clip() = default;

		virtual void addFrame(const std::shared_ptr<sf::Texture>& tex);
		virtual bool update();

		virtual bool isFinish() const { return _isFinish; };

		virtual void setOrigin(const sf::Vector2f& origin);

		virtual void setSprite(const std::shared_ptr<sf::Sprite>& sprite);

		virtual void setSpeed(const float speed);

		virtual void start();

		virtual void setCallback(const std::function<void()>& callme);
	protected:
		bool _isFinish;
		std::vector<std::shared_ptr<sf::Texture>> _container;
		std::vector < std::shared_ptr<sf::Texture>>::iterator _currentFrame;
		std::shared_ptr<sf::Sprite> _sprite;
		float _speed;
		float _curTime;
		sf::Vector2f _origin;
		std::function<void()> _callback;
	};
}