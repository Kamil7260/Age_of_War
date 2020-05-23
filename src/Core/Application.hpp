#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <nlohmann/json.hpp>

#include "Renderer.hpp"
#include "ClipManager.hpp"

using json = nlohmann::json;

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

	const base::Clip& getClip(const char* name);

	const json& getMobInfo() const {
		return _mobInfoJson;
	}

	~Application() = default;
	Application(Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;
private:

	void assetLoader();
	
	json _mobInfoJson;
	Application();	
	sf::Clock _clock;
	float _deltaTime;
	std::unique_ptr<ClipManager> _clips;
};
}