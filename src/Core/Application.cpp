#include <string>
#include <fstream>
#include "Application.hpp"
#include "ResourceManager.hpp"

#include "../GameClass/Background.hpp"
#include "../GameClass/Player.hpp"
#include "../GameClass/Enemy.hpp"


void core::Application::run()
{
	LOG_INFO("Running...");

	LOG_INFO("Init Renderer");
	auto& renderer = Renderer::getInstance();
	auto& window = renderer.getWindow();
	float camSpeed = 400.f;
	renderer.setCamSpeed(camSpeed);
	renderer.setCamScope({0.f,1150.f});
	m_windowSize = static_cast<sf::Vector2f>(renderer.getInstance().getWindow()->getSize());

	LOG_INFO("Init TextureManager");
	auto& textureManager = ResourceManager<sf::Texture>::getInstance();

	LOG_INFO("Init FontsManager");
	auto& fontManager = ResourceManager<sf::Font>::getInstance();

	LOG_INFO("Init ClipManager");
	m_clips = std::make_unique<ClipManager>();

	LOG_INFO("Loading mobinfo json");
	{
		std::ifstream reader("Data/mobInfo.json");
		if (!reader.good())
		{
			LOG_ERROR("Can not open mobinfo json file");
			return;
		}
		reader >> m_mobInfoJson;
		reader.close();
	}

	LOG_INFO("Loading cannonInfo json");
	{
		std::ifstream reader("Data/cannonsInfo.json");
		if (!reader.good())
		{
			LOG_ERROR("Can not open cannonInfo json file");
			return;
		}
		reader >> m_cannonInfoJson;
		reader.close();
	}

	m_clock.restart();
	float frameStartTime;
	
	{
		bool isLoaded = false;
		std::thread loader(&Application::assetLoader, this, std::ref(isLoaded));

		loadingScreen(isLoaded, window);
		loader.join();
		//loader.detach();
	}
	m_cursorTextures.at(0) = textureManager.get("Assets/gui/3.png");
	m_cursorTextures.at(1) = textureManager.get("Assets/gui/6.png");
	reset();

	setCursor(0);
	window->setMouseCursorVisible(false);


	auto& view = renderer.getView();

	float lastFpsTimeCheck = 0.f;
	sf::Text fpsPlayer;
	fpsPlayer.setCharacterSize(20);
	fpsPlayer.setFont(*fontManager.get("Assets/fonts/3.ttf"));
	fpsPlayer.setPosition(sf::Vector2f(10.f, 10.f));
	fpsPlayer.setFillColor(sf::Color::Red);

	fpsPlayer.setString("60");
	LOG_INFO("Main loop...");
	while (window->isOpen())
	{
		frameStartTime = m_clock.getElapsedTime().asSeconds();
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				LOG_INFO("Window has been closed");
				return;
			}
		}
		renderer.updateCollision();
		renderer.update();

		auto pos = sf::Mouse::getPosition(*window);
		if (pos.y > window->getSize().y * 0.2f)
		{
			if (pos.x < window->getSize().x * 0.1f)
			{
				renderer.move(sf::Vector2f(camSpeed * m_deltaTime, 0));
			}
			else if (pos.x > window->getSize().x * 0.9f)
			{
				renderer.move(sf::Vector2f(-camSpeed * m_deltaTime, 0));
			}
		}

		m_cursor.setPosition(static_cast<sf::Vector2f>(renderer.getMousePosition()));

		view.setCenter(sf::Vector2f(960.f,540.f));
		window->clear();
		window->setView(view);
		renderer.draw();

		window->draw(fpsPlayer);

		window->draw(m_cursor);
		window->display();
		m_deltaTime = m_clock.getElapsedTime().asSeconds() - frameStartTime;
		lastFpsTimeCheck += m_deltaTime;
		if (lastFpsTimeCheck > 0.2f)
		{
			fpsPlayer.setString(std::to_string(static_cast<int>(1.f / m_deltaTime)));
			lastFpsTimeCheck = 0.f;
		}

		if (m_deltaTime > 0.08f) {
			m_deltaTime = 0.08f;
		}

		if (m_breakLoop)
		{
			freeze();
			reset();
		}
	}
	return;
}

const base::Clip& core::Application::getClip(const std::string& name)
{
	return *m_clips->getClip(name);
}

void core::Application::setCursor(const unsigned int type)
{
	auto& k = m_cursorTextures.at(type);
	m_cursor.setOrigin(sf::Vector2f(k->getSize().x / 2.f, k->getSize().y / 2.f));
	m_cursor.setTexture(*k, true);
}

void core::Application::freezeScreen(const sf::Texture& at)
{
	m_breakLoop = true;
	m_freezeTexture = at;
}

void core::Application::assetLoader(bool &isLoaded)
{
	auto &texture = ResourceManager<sf::Texture>::getInstance();
	auto& font = ResourceManager<sf::Font>::getInstance();
	json clipInfo;
	{
		LOG_INFO("Loading clips from json");
		{
			std::ifstream reader("Data/clipLoader.json");
			if (!reader.good())
			{
				LOG_ERROR("Can not open mobinfo json file");
				return;
			}
			reader >> clipInfo;
			reader.close();
		}
	}
	std::unique_ptr<base::Clip> clip;
	int count;
	std::string path;
	std::string lpath;
	sf::Vector2f origin;
	std::string name;
	json::value_type val;
	for (auto it = clipInfo.begin(); it != clipInfo.end(); ++it)
	{
		name = (*it)["name"];
		count = (*it)["count"];
		path = (*it)["path"];
		val =  (*it)["origin"];
		origin.x = val.at(0);
		origin.y = val.at(1);
		clip = std::make_unique<base::Clip>();
		clip->setOrigin(origin);
		for (int k = 1; k <= count; ++k)
		{

			lpath = path;
			lpath += std::to_string(k);
			lpath += ".png";
			texture.loadFromFile(lpath);
			clip->addFrame(texture.get(lpath));

		}
		m_clips->addClip(std::move(clip), name);
	}

	json assetInfo;
	LOG_INFO("Loading assets from json");
	{
		std::ifstream reader("Data/assets.json");
		if (!reader.good())
		{
			LOG_ERROR("Can not open assetInfo json file");
			return;
		}
		reader >> assetInfo;
		reader.close();
	}
	std::string type;
	for (auto it = assetInfo.begin(); it != assetInfo.end(); ++it)
	{
		type = (*it)["type"];
		path = (*it)["path"];

		if (type == "Texture") {
			texture.loadFromFile(path);
		}
		else if (type == "Font") {
			font.loadFromFile(path);

		}
	}
	isLoaded = true;
}

void core::Application::loadingScreen(bool& isLoaded,const std::unique_ptr<sf::RenderWindow>& window)
{
	std::array<sf::Texture,37> tex;
	for (int k = 1; k <= 37; ++k)
	{
		tex.at(k-1).loadFromFile("Assets/loading/" + std::to_string(k) + ".jpg");
	}

	sf::Sprite sprite;
	auto texk = static_cast<sf::Vector2f>(tex[0].getSize());
	auto wink = static_cast<sf::Vector2f>(window->getSize());

	sprite.setScale(sf::Vector2f(wink.x / texk.x, wink.y / texk.y));
	size_t currentFrame = 0;
	sf::Clock clock;
	float frameStartTime;
	float deltaTime = 1 / 60.f;
	float currentTime = 0.f;
	while (window->isOpen() && !isLoaded)
	{
		currentTime += deltaTime;

		frameStartTime = clock.getElapsedTime().asSeconds();
		{
			sf::Event event;
			while (window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					isLoaded = true;
					window->close();
				}
			}

			if (currentTime > 0.05f)
			{
				currentTime = 0.f;
				++currentFrame;

				if (currentFrame < tex.size())
				{
					sprite.setTexture(tex.at(currentFrame));
				}
				else currentFrame = 0;
			}

			window->clear();
			window->draw(sprite);
			window->display();
			deltaTime = clock.getElapsedTime().asSeconds() - frameStartTime;
		}
	}
}

void core::Application::reset()
{
	auto& renderer = core::Renderer::getInstance();
	renderer.resetPosition(sf::Vector2f(0.f, 0.f));
	auto k = core::ResourceManager<sf::Texture>::getInstance().get("Assets/background/1.png");
	if (k == nullptr) {
		LOG_ERROR("Resource manager get(Assets/background/1.png) -> nullptr");
	}
	else {
		std::unique_ptr<BackGround> bcg = std::make_unique<BackGround>();
		bcg->setPosition(sf::Vector2f(-50.f, 0.f));
		bcg->setTexture(*k);
		bcg->setScale(sf::Vector2f(1.f, 1.07f));
		renderer.addObject(std::move(bcg), base::object_type::background);
	}
	k = core::ResourceManager<sf::Texture>::getInstance().get("Assets/gui/4.png");
	if (k == nullptr) {
		LOG_ERROR("Resource manager get(Assets/gui/4.png) -> nullptr");
	}
	else {
		std::unique_ptr<BackGround> bcg = std::make_unique<BackGround>();
		bcg = std::make_unique<BackGround>();
		bcg->dynamicDraw(false);
		bcg->setTexture(*k);
		bcg->setPosition(sf::Vector2f(200.f, 0.f));
		renderer.addObject(std::move(bcg), base::object_type::gui);
	}


	std::unique_ptr<Player> player = std::make_unique<Player>();
	player->setPosition(sf::Vector2f(140, 870));
	renderer.addObject(std::move(player), base::object_type::actor);

	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	enemy->setPosition(sf::Vector2f(2900, 870));
	renderer.addEnemyObject(std::move(enemy));
}

void core::Application::freeze()
{
	sf::Sprite sprite;

	auto& window = core::Renderer::getInstance().getWindow();
	sprite.setTexture(m_freezeTexture);
	auto texk = static_cast<sf::Vector2f>(m_freezeTexture.getSize());
	//auto wink = static_cast<sf::Vector2f>(window->getSize());
	auto& view = core::Renderer::getInstance().getView();
	sprite.setScale(sf::Vector2f(1920.f / texk.x, 1080.f / texk.y));
	sprite.setPosition(0.f, 0.f);
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
			if (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::KeyReleased)
			{
				core::Renderer::getInstance().forceClear();
				m_breakLoop = false;
				return;
			}
			
		}
		m_cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)));
		view.setCenter(sf::Vector2f(960.f, 540.f));
		window->clear();
		window->setView(view);
		window->draw(sprite);
		window->draw(m_cursor);
		window->display();
	}
}

core::Application::Application()
	:m_deltaTime(0.f), m_breakLoop(false)
{}