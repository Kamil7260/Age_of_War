#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <memory>
namespace base {

	class Clip {
	public:
		Clip(float speed = 0.f);
		Clip(Clip&);
		Clip(Clip&&) noexcept;
		Clip& operator=(Clip&);
		Clip& operator=(Clip&&) noexcept;

		~Clip() = default;

		virtual bool addFrame(const std::shared_ptr<sf::Texture>& tex);
		virtual bool update();

		virtual bool isFinish() const { return _isFinish; };

		virtual void setSprite(const std::shared_ptr<sf::Sprite>& sprite);

		virtual void start();
	protected:
		bool _isFinish;
		std::vector<std::shared_ptr<sf::Texture>> _container;
		std::vector < std::shared_ptr<sf::Texture>>::iterator _currentFrame;
		std::shared_ptr<sf::Sprite> _sprite;
		float _speed;
		float _curTime;
	};
}