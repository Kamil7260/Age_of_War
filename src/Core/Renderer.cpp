#include "Renderer.hpp"
#include "Application.hpp"

#include <fstream>
void core::Renderer::addObject(std::unique_ptr<base::Actor> actor, base::object_type layer)
{
	actor->setType(layer);
	actor->setTeam(base::team::player);
	m_queue.push_back(std::make_pair(std::move(actor), layer));
}

void core::Renderer::forceAddObject(std::unique_ptr<base::Actor> actor, base::object_type layer)
{
	switch (layer)
	{
	case base::object_type::actor:
		m_actor.push_back(std::move(actor));
		break;
	case base::object_type::background:
		m_backGround.push_back(std::move(actor));
		break;
	case base::object_type::gui:
		m_gui.push_back(std::move(actor));
		break;
	}
}

void core::Renderer::addEnemyObject(std::unique_ptr<base::Actor> actor)
{
	actor->setType(base::object_type::actor);
	actor->setTeam(base::team::enemy);
	m_enemyQueue.push_back(std::move(actor));
}

void core::Renderer::update()
{
	try {
		for (auto k = m_actor.begin(); k != m_actor.end(); ++k)
		{
			(*k)->onUpdate();
		}
		for (auto k = m_enemyActor.begin(); k != m_enemyActor.end(); ++k)
		{
			(*k)->onUpdate();
		}
		for (auto k = m_gui.begin(); k != m_gui.end(); ++k)
		{
			(*k)->onUpdate();
		}
		for (auto k = m_bulletActor.begin(); k != m_bulletActor.end(); ++k)
		{
			(*k)->onUpdate();
		}
		for (auto k = m_enemyBulletActor.begin(); k != m_enemyBulletActor.end(); ++k)
		{
			(*k)->onUpdate();
		}

		if (m_shouldBeErase)
			eraseNoActive();

		insertQueue();
	}
	catch (...)
	{
		LOG_ERROR("Exception at onUpdate thrown");
		Application::getInstance().freezeScreen();
	}
}

void core::Renderer::draw()
{
	try {
		for (auto k = m_backGround.begin(); k != m_backGround.end(); ++k)
		{
			m_window->draw((*(*k)));
		}
		for (auto k = m_actor.begin(); k != m_actor.end(); ++k)
		{
			m_window->draw((*(*k)));
		}
		for (auto k = m_enemyActor.begin(); k != m_enemyActor.end(); ++k)
		{
			m_window->draw((*(*k)));
		}
		for (auto k = m_gui.begin(); k != m_gui.end(); ++k)
		{
			m_window->draw((*(*k)));
		}
		for (auto k = m_bulletActor.begin(); k != m_bulletActor.end(); ++k)
		{
			m_window->draw((*(*k)));
		}
		for (auto k = m_enemyBulletActor.begin(); k != m_enemyBulletActor.end(); ++k)
		{
			m_window->draw((*(*k)));
		}
	}
	catch (...)
	{
		LOG_ERROR("Exception at draw thrown");
		Application::getInstance().freezeScreen();
	}
}

void core::Renderer::clearNoActive()
{
	m_shouldBeErase = true;
}

std::unique_ptr<base::Actor>& core::Renderer::isEnemyInRange(const sf::Vector2f& myPosition, const int myRange, const base::team& myTeam)
{
	if (myTeam == base::team::player)
	{
		if (m_enemyActor.empty())
			return m_placeHolder;

		if (m_enemyActor.size() == 1)
		{
			auto it = m_enemyActor.begin();
			if((*it)->isColliderActive())
			if (myPosition.x + myRange > (*it)->getPosition().x - (*it)->getCollider().left)
			{
				return *it;
			}
		}
		else
		{
			auto it = m_enemyActor.begin() + 1;
			for (; it != m_enemyActor.end(); ++it)
			{
				if ((*it)->isColliderActive())
				if (myPosition.x + myRange > (*it)->getPosition().x - (*it)->getCollider().left)
				{
					return *it;
				}
			}
		}
	}
	else if (myTeam == base::team::enemy)
	{

		if (m_actor.empty())
			return m_placeHolder;

		if (m_actor.size() == 1)
		{
			auto it = m_actor.begin();
			if ((*it)->isColliderActive())
			if (myPosition.x - myRange < (*it)->getPosition().x + (*it)->getCollider().right)
			{
				return *it;
			}
		}
		else
		{
			auto it = m_actor.begin() + 1;
			for (; it != m_actor.end(); ++it)
			{
				if ((*it)->isColliderActive())
				if (myPosition.x - myRange < (*it)->getPosition().x + (*it)->getCollider().right)
				{
					return *it;
				}
			}
		}

	}
	return m_placeHolder;
}

void core::Renderer::updateCollision()
{
	m_mousePosition = sf::Mouse::getPosition(*m_window);

	if(!m_actor.empty() && !m_enemyActor.empty())
	collisionBetween(getLastColliderActor(m_actor), getLastColliderActor(m_enemyActor));

	if (!m_actor.empty())
	{
		auto left = m_actor.begin();
		auto right = m_actor.begin() + 1;
		if (right != m_actor.end())
		{
			collisionBetweenBack(*left, *right);
			onMouse(*right);
			left = getNextActiveCollider(left, m_actor.end());
			right = getNextActiveCollider(right, m_actor.end());
			if (left == right && right != m_actor.end()) {
				right = getNextActiveCollider(right, m_actor.end());
			}
			for (; right != m_actor.end();)
			{
				collisionBetweenBack(*left, *right);
				onMouse(*right);
				left = getNextActiveCollider(left, m_actor.end());
				right = getNextActiveCollider(right, m_actor.end());
			}
		}
	}
	if (!m_enemyActor.empty())
	{
		auto left = m_enemyActor.begin();
		auto right = m_enemyActor.begin() + 1;
		if (right != m_enemyActor.end())
		{
			collisionBetweenBack(*left, *right);
			onMouse(*right);
			left = getNextActiveCollider(left, m_enemyActor.end());
			right = getNextActiveCollider(right, m_enemyActor.end());
			if (left == right && right != m_enemyActor.end()) {
				right = getNextActiveCollider(right, m_enemyActor.end());
			}
			for (; right != m_enemyActor.end();)
			{
				collisionBetweenBack(*left, *right);
				onMouse(*right);
				left = getNextActiveCollider(left, m_enemyActor.end());
				right = getNextActiveCollider(right, m_enemyActor.end());
			}
		}
	}
	for (auto k = m_gui.begin(); k != m_gui.end(); ++k)
	{
		onMouse((*k));
	}

	for (auto k = m_bulletActor.begin(); k != m_bulletActor.end(); ++k)
	{
		for (auto it = m_enemyActor.begin(); it != m_enemyActor.end(); ++it)
		{
			if ((*it)->isColliderActive())
			if (collisionBetween(*k, *it))
			{
				break;
			}
		}
	}

	for (auto k = m_enemyBulletActor.begin(); k != m_enemyBulletActor.end(); ++k)
	{
		for (auto it = m_actor.begin(); it != m_actor.end(); ++it)
		{
			if((*it)->isColliderActive())
			if (collisionBetween(*k, *it))
			{
				break;
			}
		}
	}
}

void core::Renderer::move(const sf::Vector2f& delta)
{
	if (delta.x > 0)
	{
		if (delta.x + m_position.x > m_scope.x)
			return;
	}
	else {
		if (delta.x + m_position.x < -m_scope.y)
			return;
	}
	m_position += delta;

	for (auto it = m_backGround.begin(); it != m_backGround.end(); ++it)
	{
		(*it)->move(delta);
	}
	for (auto it = m_actor.begin(); it != m_actor.end(); ++it)
	{
		(*it)->move(delta);
	}
	for (auto it = m_enemyActor.begin(); it != m_enemyActor.end(); ++it)
	{
		(*it)->move(delta);
	}
	for (auto it = m_queue.begin(); it != m_queue.end(); ++it)
	{
		it->first->move(delta);
	}
	for (auto it = m_bulletActor.begin(); it != m_bulletActor.end(); ++it)
	{
		(*it)->move(delta);
	}
	for (auto it = m_enemyBulletActor.begin(); it != m_enemyBulletActor.end(); ++it)
	{
		(*it)->move(delta);
	}

	for (auto it = m_gui.begin(); it != m_gui.end(); ++it)
	{
		if ((*it)->isDynamic())
		{
			(*it)->move(delta);
		}
	}
}

std::unique_ptr<base::Actor>& core::Renderer::find(const std::string& tag)
{
	for (auto it = m_gui.begin(); it != m_gui.end(); ++it)
	{
		if ((*it)->getTag() == tag)
			return *it;
	}
	for (auto it = m_backGround.begin(); it != m_backGround.end(); ++it)
	{
		if ((*it)->getTag() == tag)
			return *it;
	}
	for (auto it = m_enemyActor.begin(); it != m_enemyActor.end(); ++it)
	{
		if ((*it)->getTag() == tag)
			return *it;
	}
	for (auto it = m_actor.begin(); it != m_actor.end(); ++it)
	{
		if ((*it)->getTag() == tag)
			return *it;
	}
	return m_placeHolder;
}

std::unique_ptr<base::Actor> core::Renderer::findAndRemove(const std::string& tag)
{
	for (auto it = m_gui.begin(); it != m_gui.end(); ++it)
	{
		if ((*it)->getTag() == tag)
		{
			auto result = std::move(*it);
			m_gui.erase(it);
			return std::move(result);
		}
	}
	for (auto it = m_backGround.begin(); it != m_backGround.end(); ++it)
	{
		if ((*it)->getTag() == tag)
		{
			auto result = std::move(*it);
			m_backGround.erase(it);
			return std::move(result);
		}
	}
	for (auto it = m_actor.begin(); it != m_actor.end(); ++it)
	{
		if ((*it)->getTag() == tag)
		{
			auto result = std::move(*it);
			m_actor.erase(it);
			return std::move(result);
		}
	}
	for (auto it = m_enemyActor.begin(); it != m_enemyActor.end(); ++it)
	{
		if ((*it)->getTag() == tag)
		{
			auto result = std::move(*it);
			m_enemyActor.erase(it);
			return std::move(result);
		}
	}
	return nullptr;
}

void core::Renderer::forceClear()
{
	if (!m_queue.empty())
		m_queue.clear();
	if (!m_enemyQueue.empty())
		m_enemyQueue.clear();
	if (!m_bulletQueue.empty())
		m_bulletQueue.clear();
	if (!m_backGround.empty())
		m_backGround.clear();
	if (!m_actor.empty())
		m_actor.clear();
	if (!m_bulletActor.empty())
		m_bulletActor.clear();
	if (!m_enemyBulletActor.empty())
		m_enemyBulletActor.clear();
	if (!m_enemyActor.empty())
		m_enemyActor.clear();
	if (!m_gui.empty())
		m_gui.clear();
}

std::unique_ptr<base::Actor>& core::Renderer::getLastColliderActor(std::vector<std::unique_ptr<base::Actor>>& actor)
{
	if (actor.size() <= 1)
		return *actor.begin();

	for (auto it = actor.begin() + 1; it != actor.end(); ++it)
	{
		if ((*it)->isColliderActive())
			return *it;
	}
	
	return *actor.begin();
}

void core::Renderer::eraseNoActive()
{
	auto fun = [](const std::unique_ptr<base::Actor>& ptr)-> bool {
		return !ptr->isActive();
	};

	auto it = std::remove_if(m_backGround.begin(), m_backGround.end(), fun);
	if(!m_backGround.empty())
	m_backGround.erase(it, m_backGround.end());

	if (!m_actor.empty())
	{
		it = std::remove_if(m_actor.begin(), m_actor.end(), fun);
		m_actor.erase(it, m_actor.end());
	}
	if (!m_enemyActor.empty())
	{
		it = std::remove_if(m_enemyActor.begin(), m_enemyActor.end(), fun);
		m_enemyActor.erase(it, m_enemyActor.end());
	}
	if (!m_gui.empty())
	{
		it = std::remove_if(m_gui.begin(), m_gui.end(), fun);
		m_gui.erase(it, m_gui.end());
	}
	if (!m_bulletActor.empty())
	{
		it = std::remove_if(m_bulletActor.begin(), m_bulletActor.end(), fun);
		m_bulletActor.erase(it, m_bulletActor.end());
	}
	if (!m_enemyBulletActor.empty())
	{
		it = std::remove_if(m_enemyBulletActor.begin(), m_enemyBulletActor.end(), fun);
		m_enemyBulletActor.erase(it, m_enemyBulletActor.end());
	}
	m_shouldBeErase = false;
}

void core::Renderer::insertQueue()
{
	for(auto it = m_queue.begin(); it != m_queue.end(); ++it)
	{
		switch (it->second)
		{
		case base::object_type::actor:
			m_actor.push_back(std::move(it->first));
			break;
		case base::object_type::background:
			m_backGround.push_back(std::move(it->first));
			break;
		case base::object_type::gui:
			m_gui.push_back(std::move(it->first));
			break;
		}
	}
	m_queue.clear();

	for (auto it = m_enemyQueue.begin(); it != m_enemyQueue.end(); ++it)
	{
		m_enemyActor.push_back(std::move(*it));
	}
	m_enemyQueue.clear();

	for (auto it = m_bulletQueue.begin(); it != m_bulletQueue.end(); ++it)
	{
		if ((*it)->getTeam() == base::team::player)
		{
			m_bulletActor.push_back(std::move(*it));
		}
		else if ((*it)->getTeam() == base::team::enemy)
		{
			m_enemyBulletActor.push_back(std::move(*it));
		}
	}
	m_bulletQueue.clear();
}

bool core::Renderer::collisionBetween(std::unique_ptr<base::Actor>& left, std::unique_ptr<base::Actor>& right) const
{
	auto leftCollider = left->getCollider();
	auto rightCollider = right->getCollider();

	if (left->getPosition().x + leftCollider.right >= right->getPosition().x - rightCollider.left
		&& left->getPosition().x - leftCollider.left <= right->getPosition().x + rightCollider.right)
	{
		if (left->getPosition().y - leftCollider.up <= right->getPosition().y + rightCollider.down
			&& left->getPosition().y + leftCollider.down >= right->getPosition().y - rightCollider.up)
		{
			left->onCollision(right);
			right->onCollision(left);
			return true;
		}
	}
	return false;
}

bool core::Renderer::collisionBetweenBack(std::unique_ptr<base::Actor>& left, std::unique_ptr<base::Actor>& right) const
{
	auto leftCollider = left->getCollider();
	auto rightCollider = right->getCollider();

	if (left->getPosition().x + leftCollider.right >= right->getPosition().x - rightCollider.left
		&& left->getPosition().x - leftCollider.left <= right->getPosition().x + rightCollider.right)
	{
		if (left->getPosition().y - leftCollider.up <= right->getPosition().y + rightCollider.down
			&& left->getPosition().y + leftCollider.down >= right->getPosition().y - rightCollider.up)
		{
		//	left->onCollision(right);
			right->onCollision(left);
			return true;
		}
	}
	return false;
}

std::vector<std::unique_ptr<base::Actor>>::iterator& core::Renderer::getNextActiveCollider(std::vector<std::unique_ptr<base::Actor>>::iterator& current, const std::vector<std::unique_ptr<base::Actor>>::iterator& last)
{
	++current;
	for (current; current != last; ++current)
	{
		if ((*current)->isColliderActive())
		{
			return current;
		}
	}
	return current;
}

void core::Renderer::onMouse(const std::unique_ptr<base::Actor>& source) const
{
	auto& collider = source->getCollider();
	auto& position = source->getPosition();
	if (position.x + collider.right > m_mousePosition.x &&
		position.x - collider.left < m_mousePosition.x)
	{
		if (position.y + collider.down > m_mousePosition.y &&
			position.y - collider.up < m_mousePosition.y)
		{
			source->onMouseCollision(sf::Mouse::isButtonPressed(sf::Mouse::Left));
		}
	}
}

void core::Renderer::addBullet(std::unique_ptr<base::Actor> actor)
{
	m_bulletQueue.push_back(std::move(actor));
}

core::Renderer::Renderer()
{
	json file;
	sf::Vector2u res = { 1366,768 };
	LOG_INFO("Reading resolution from user options")
	{
		std::ifstream reader("Data/options.json");
		if (!reader.good())
		{
			LOG_ERROR("Can not open options json file");
		}
		else {
			reader >> file;
			reader.close();

			auto k = file["resolution"];
			if (k != nullptr)
			{
				res.x = k.at(0);
				res.y = k.at(1);
			}
			else
				LOG_WARNING("Incomplete resolution");
		}
	}


	LOG_INFO("Creating window...");
	m_view = sf::View(sf::Vector2f(0, 0), sf::Vector2f(1920, 1080));
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(res.x,res.y), "Age of War");
	m_window->setFramerateLimit(60);
	m_placeHolder = nullptr;
	m_camSpeed = 0;
	m_scope = { 0.f,0.f };
	m_position = { 0.f,0.f };

	sf::Image image;
	if(image.loadFromFile("ic.png"))
		m_window->setIcon(32, 32, image.getPixelsPtr());

}
