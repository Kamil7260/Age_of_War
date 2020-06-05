#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <thread>
#include <mutex>

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

	const base::Clip& getClip(const std::string& name);

	const json& getMobInfo() const {
		return _mobInfoJson;
	}
	const json& getCannonInfo() const {
		return _cannonInfoJson;
	}

	virtual void setCursor(const unsigned int type);

	virtual void freezeScreen(const sf::Texture& at);

	~Application() = default;
	Application(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;
private:
	void assetLoader(bool& isLoaded);
	void loadingScreen(bool& isLoaded,const std::unique_ptr<sf::RenderWindow>& window);

	void reset();
	void freeze();

	json _mobInfoJson;
	json _cannonInfoJson;
	Application();	
	sf::Clock _clock;
	float _deltaTime;
	std::unique_ptr<ClipManager> _clips;
	sf::Sprite _cursor;
	std::array<std::shared_ptr<sf::Texture>, 2> _cursorTextures;
	sf::Texture _freezeTexture;
	bool _breakLoop;
	sf::Vector2f _windowSize;
};
}