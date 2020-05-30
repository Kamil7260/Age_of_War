#include "Renderer.hpp"

void core::Renderer::addObject(std::unique_ptr<base::Actor> actor, base::object_type layer)
{
	actor->setType(layer);
	actor->setTeam(base::team::player);
	_queue.push_back(std::make_pair(std::move(actor), layer));
}

void core::Renderer::forceAddObject(std::unique_ptr<base::Actor> actor, base::object_type layer)
{
	switch (layer)
	{
	case base::object_type::actor:
		_actor.push_back(std::move(actor));
		break;
	case base::object_type::background:
		_backGround.push_back(std::move(actor));
		break;
	case base::object_type::gui:
		_gui.push_back(std::move(actor));
		break;
	}
}

void core::Renderer::addEnemyObject(std::unique_ptr<base::Actor> actor)
{
	actor->setType(base::object_type::actor);
	actor->setTeam(base::team::enemy);
	_enemyQueue.push_back(std::move(actor));
}

void core::Renderer::update()
{
	for (auto k = _actor.begin(); k != _actor.end(); ++k)
	{
		(*k)->onUpdate();
	}
	for (auto k = _enemyActor.begin(); k != _enemyActor.end(); ++k)
	{
		(*k)->onUpdate();
	}
	for (auto k = _gui.begin(); k != _gui.end(); ++k)
	{
		(*k)->onUpdate();
	}
	for (auto k = _bulletActor.begin(); k != _bulletActor.end(); ++k)
	{
		(*k)->onUpdate();
	}
	for (auto k = _enemyBulletActor.begin(); k != _enemyBulletActor.end(); ++k)
	{
		(*k)->onUpdate();
	}

	if (_shouldBeErase)
		eraseNoActive();

	insertQueue();
}

void core::Renderer::draw()
{
	for (auto k = _backGround.begin(); k != _backGround.end(); ++k)
	{
		_window->draw((*(*k)));
	}
	for (auto k = _actor.begin(); k != _actor.end(); ++k)
	{
		_window->draw((*(*k)));
	}
	for (auto k = _enemyActor.begin(); k != _enemyActor.end(); ++k)
	{
		_window->draw((*(*k)));
	}
	for (auto k = _gui.begin(); k != _gui.end(); ++k)
	{
		_window->draw((*(*k)));
	}
	for (auto k = _bulletActor.begin(); k != _bulletActor.end(); ++k)
	{
		_window->draw((*(*k)));
	}
	for (auto k = _enemyBulletActor.begin(); k != _enemyBulletActor.end(); ++k)
	{
		_window->draw((*(*k)));
	}
}

void core::Renderer::clearNoActive()
{
	_shouldBeErase = true;
}

std::unique_ptr<base::Actor>& core::Renderer::isEnemyInRange(const sf::Vector2f& myPosition, const float myRange, const base::team& myTeam)
{
	if (myTeam == base::team::player)
	{
		if (_enemyActor.empty())
			return _placeHolder;

		if (_enemyActor.size() == 1)
		{
			auto it = _enemyActor.begin();
			if (myPosition.x + myRange > (*it)->getPosition().x)
			{
				if (myPosition.x < (*it)->getPosition().x)
				{
					return *it;
				}
			}
		}
		else for (auto it = _enemyActor.begin() + 1; it != _enemyActor.end(); ++it)
		{
			if((*it)->isColliderActive())
				if (myPosition.x + myRange > (*it)->getPosition().x)
				{
					if (myPosition.x < (*it)->getPosition().x)
					{
						return *it;
					}
				}
		}
		auto it = _enemyActor.begin();
		if (myPosition.x + myRange > (*it)->getPosition().x)
		{
			if (myPosition.x < (*it)->getPosition().x)
			{
				return *it;
			}
		}
	}
	else if (myTeam == base::team::enemy)
	{
		if (_actor.empty())
			return _placeHolder;

		if (_actor.size() == 1)
		{
			auto it = _actor.begin();
			if (myPosition.x + myRange > (*it)->getPosition().x)
			{
				if (myPosition.x < (*it)->getPosition().x)
				{
					return *it;
				}
			}
		}
		else for (auto it = _actor.begin() + 1; it != _actor.end(); ++it)
		{
			if ((*it)->isColliderActive())
				if (myPosition.x - myRange < (*it)->getPosition().x)
				{
					if (myPosition.x > (*it)->getPosition().x)
					{
						return *it;
					}
				}
		}
		auto it = _actor.begin();
		if (myPosition.x + myRange > (*it)->getPosition().x)
		{
			if (myPosition.x < (*it)->getPosition().x)
			{
				return *it;
			}
		}
	}
	return _placeHolder;
}

void core::Renderer::updateCollision()
{
	_mousePosition = sf::Mouse::getPosition(*_window);

	if(!_actor.empty() && !_enemyActor.empty())
	collisionBetween(getLastColliderActor(_actor), getLastColliderActor(_enemyActor));

	if (!_actor.empty())
	{
		auto left = _actor.begin();
		auto right = _actor.begin() + 1;
		if (right != _actor.end())
		{
			collisionBetween(*left, *right);
			onMouse(*right);
			left = getNextActiveCollider(left, _actor.end());
			right = getNextActiveCollider(right, _actor.end());
			if (left == right && right != _actor.end()) {
				right = getNextActiveCollider(right, _actor.end());
			}
			for (; right != _actor.end();)
			{
				collisionBetween(*left, *right);
				onMouse(*right);
				left = getNextActiveCollider(left, _actor.end());
				right = getNextActiveCollider(right, _actor.end());
			}
		}
	}
	if (!_enemyActor.empty())
	{
		auto left = _enemyActor.begin();
		auto right = _enemyActor.begin() + 1;
		if (right != _enemyActor.end())
		{
			collisionBetween(*left, *right);
			onMouse(*right);
			left = getNextActiveCollider(left, _enemyActor.end());
			right = getNextActiveCollider(right, _enemyActor.end());
			if (left == right && right != _enemyActor.end()) {
				right = getNextActiveCollider(right, _enemyActor.end());
			}
			for (; right != _enemyActor.end();)
			{
				collisionBetween(*left, *right);
				onMouse(*right);
				left = getNextActiveCollider(left, _enemyActor.end());
				right = getNextActiveCollider(right, _enemyActor.end());
			}
		}
	}
	for (auto k = _gui.begin(); k != _gui.end(); ++k)
	{
		onMouse((*k));
	}

	for (auto k = _bulletActor.begin(); k != _bulletActor.end(); ++k)
	{
		for (auto it = _enemyActor.begin(); it != _enemyActor.end(); ++it)
		{
			if (collisionBetween(*k, *it))
			{
				(*k)->onCollision(*it);
				(*it)->onCollision(*k);
				break;
			}
		}
	}

	for (auto k = _enemyBulletActor.begin(); k != _enemyBulletActor.end(); ++k)
	{
		for (auto it = _actor.begin(); it != _actor.end(); ++it)
		{
			if((*it)->isColliderActive())
			if (collisionBetween(*k, *it))
			{
				(*k)->onCollision(*it);
				(*it)->onCollision(*k);
				break;
			}
		}
	}
}

void core::Renderer::move(const sf::Vector2f& delta)
{
	if (delta.x > 0)
	{
		if (delta.x + _position.x > _scope.x)
			return;
	}
	else {
		if (delta.x + _position.x < -_scope.y)
			return;
	}
	_position += delta;

	for (auto it = _backGround.begin(); it != _backGround.end(); ++it)
	{
		(*it)->move(delta);
	}
	for (auto it = _actor.begin(); it != _actor.end(); ++it)
	{
		(*it)->move(delta);
	}
	for (auto it = _enemyActor.begin(); it != _enemyActor.end(); ++it)
	{
		(*it)->move(delta);
	}
	for (auto it = _queue.begin(); it != _queue.end(); ++it)
	{
		it->first->move(delta);
	}
	for (auto it = _bulletActor.begin(); it != _bulletActor.end(); ++it)
	{
		(*it)->move(delta);
	}
	for (auto it = _enemyBulletActor.begin(); it != _enemyBulletActor.end(); ++it)
	{
		(*it)->move(delta);
	}
}

std::unique_ptr<base::Actor>& core::Renderer::find(const std::string& tag)
{
	for (auto it = _gui.begin(); it != _gui.end(); ++it)
	{
		if ((*it)->getTag() == tag)
			return *it;
	}
	for (auto it = _backGround.begin(); it != _backGround.end(); ++it)
	{
		if ((*it)->getTag() == tag)
			return *it;
	}
	for (auto it = _actor.begin(); it != _actor.end(); ++it)
	{
		if ((*it)->getTag() == tag)
			return *it;
	}
	for (auto it = _enemyActor.begin(); it != _enemyActor.end(); ++it)
	{
		if ((*it)->getTag() == tag)
			return *it;
	}
	return _placeHolder;
}

std::unique_ptr<base::Actor> core::Renderer::findAndRemove(const std::string& tag)
{
	for (auto it = _gui.begin(); it != _gui.end(); ++it)
	{
		if ((*it)->getTag() == tag)
		{
			auto result = std::move(*it);
			_gui.erase(it);
			return std::move(result);
		}
	}
	for (auto it = _backGround.begin(); it != _backGround.end(); ++it)
	{
		if ((*it)->getTag() == tag)
		{
			auto result = std::move(*it);
			_backGround.erase(it);
			return std::move(result);
		}
	}
	for (auto it = _actor.begin(); it != _actor.end(); ++it)
	{
		if ((*it)->getTag() == tag)
		{
			auto result = std::move(*it);
			_actor.erase(it);
			return std::move(result);
		}
	}
	for (auto it = _enemyActor.begin(); it != _enemyActor.end(); ++it)
	{
		if ((*it)->getTag() == tag)
		{
			auto result = std::move(*it);
			_enemyActor.erase(it);
			return std::move(result);
		}
	}
	return nullptr;
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

	auto it = std::remove_if(_backGround.begin(), _backGround.end(), fun);
	if(!_backGround.empty())
	_backGround.erase(it, _backGround.end());

	if (!_actor.empty())
	{
		it = std::remove_if(_actor.begin(), _actor.end(), fun);
		_actor.erase(it, _actor.end());
	}
	if (!_enemyActor.empty())
	{
		it = std::remove_if(_enemyActor.begin(), _enemyActor.end(), fun);
		_enemyActor.erase(it, _enemyActor.end());
	}
	if (!_gui.empty())
	{
		it = std::remove_if(_gui.begin(), _gui.end(), fun);
		_gui.erase(it, _gui.end());
	}
	if (!_bulletActor.empty())
	{
		it = std::remove_if(_bulletActor.begin(), _bulletActor.end(), fun);
		_bulletActor.erase(it, _bulletActor.end());
	}
	if (!_enemyBulletActor.empty())
	{
		it = std::remove_if(_enemyBulletActor.begin(), _enemyBulletActor.end(), fun);
		_enemyBulletActor.erase(it, _enemyBulletActor.end());
	}
	_shouldBeErase = false;
}

void core::Renderer::insertQueue()
{
	for(auto it = _queue.begin(); it != _queue.end(); ++it)
	{
		switch (it->second)
		{
		case base::object_type::actor:
			_actor.push_back(std::move(it->first));
			break;
		case base::object_type::background:
			_backGround.push_back(std::move(it->first));
			break;
		case base::object_type::gui:
			_gui.push_back(std::move(it->first));
			break;
		}
	}
	_queue.clear();

	for (auto it = _enemyQueue.begin(); it != _enemyQueue.end(); ++it)
	{
		_enemyActor.push_back(std::move(*it));
	}
	_enemyQueue.clear();

	for (auto it = _bulletQueue.begin(); it != _bulletQueue.end(); ++it)
	{
		if ((*it)->getTeam() == base::team::player)
		{
			_bulletActor.push_back(std::move(*it));
		}
		else if ((*it)->getTeam() == base::team::enemy)
		{
			_enemyBulletActor.push_back(std::move(*it));
		}
	}
	_bulletQueue.clear();
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
	if (position.x + collider.right > _mousePosition.x &&
		position.x - collider.left < _mousePosition.x)
	{
		if (position.y + collider.down > _mousePosition.y &&
			position.y - collider.up < _mousePosition.y)
		{
			source->onMouseCollision(sf::Mouse::isButtonPressed(sf::Mouse::Left));
		}
	}
}

void core::Renderer::addBullet(std::unique_ptr<base::Actor> actor)
{
	_bulletQueue.push_back(std::move(actor));
}

core::Renderer::Renderer()
{
	LOG_INFO("Creating window...");
	_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1920, 1080), "Age of War");
	_window->setFramerateLimit(60);
	_placeHolder = nullptr;
	_camSpeed = 0;
	_scope = { 0.f,0.f };
	_position = { 0.f,0.f };
}
