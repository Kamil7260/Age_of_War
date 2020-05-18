#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Renderer.hpp"
#include "ClipManager.hpp"
namespace core {

class Application final {
public:

	static Application& getInstance() {
		static Application app;
		return app;
	}
	float getTime() const { return _deltaTime; }
	const sf::Clock& getClock() {
		return _clock;
	}

	void run();

	const std::shared_ptr<base::Clip> getClip(const char* name);

	~Application() = default;
	Application(Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;
private:

	void assetLoader();
	
	Application();	
	sf::Clock _clock;
	float _deltaTime;
	std::unique_ptr<ClipManager> _clips;
};
}