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
	float getTime() const { return m_deltaTime; }
	const sf::Clock& getClock() {
		return m_clock;
	}

	void run();

	const base::Clip& getClip(const std::string& name);

	const json& getMobInfo() const {
		return m_mobInfoJson;
	}
	const json& getCannonInfo() const {
		return m_cannonInfoJson;
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

	json m_mobInfoJson;
	json m_cannonInfoJson;
	Application();	
	sf::Clock m_clock;
	float m_deltaTime;
	std::unique_ptr<ClipManager> m_clips;
	sf::Sprite m_cursor;
	std::array<std::shared_ptr<sf::Texture>, 2> m_cursorTextures;
	sf::Texture m_freezeTexture;
	bool m_breakLoop;
	sf::Vector2f m_windowSize;
};
}