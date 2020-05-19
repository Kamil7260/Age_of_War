#include "Renderer.hpp"

void core::Renderer::addObject(std::unique_ptr<base::Actor> actor, base::object_type layer)
{
	clearNoActive();
	switch (layer)
	{
	case base::object_type::actor:
		actor->setType(layer);
		_actor.push_back(std::move(actor));
		break;
	case base::object_type::background:
		actor->setType(layer);
		_backGround.push_back(std::move(actor));
		break;
	case base::object_type::gui:
		actor->setType(layer);
		_gui.push_back(std::move(actor));
		break;
	}
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
}

void core::Renderer::clearNoActive()
{
	auto fun = [](const std::unique_ptr<base::Actor>& ptr)-> bool {
		return !ptr->isActive();
	};

	auto it1 = std::remove_if(_backGround.begin(), _backGround.end(), fun);
	_backGround.erase(it1, _backGround.end());

	auto it2 = std::remove_if(_actor.begin(), _actor.end(), fun);
	_actor.erase(it2, _actor.end());

	auto it3 = std::remove_if(_enemyActor.begin(), _enemyActor.end(), fun);
	_enemyActor.erase(it3, _enemyActor.end());

	auto it4 = std::remove_if(_gui.begin(), _gui.end(), fun);
	_gui.erase(it4, _gui.end());
}

void core::Renderer::updateCollision()
{
	if (!_actor.empty() && !_enemyActor.empty())
	{
		auto& left = _actor.back();
		auto& right = _enemyActor.back();
		auto leftCollider = left->getCollider();
		auto rightCollider = right->getCollider();

		if (left->getPosition().x + leftCollider.right > right->getPosition().x - rightCollider.left
			&& left->getPosition().x - leftCollider.left < right->getPosition().x + rightCollider.right)
		{
			if (left->getPosition().y + leftCollider.up > right->getPosition().y - rightCollider.down
				&& left->getPosition().y - leftCollider.down < right->getPosition().y + rightCollider.up)
			{
				left->onCollision(right);
				right->onCollision(left);
			}
		}
	}
	if (!_actor.empty())
	{
		auto left = _actor.begin();
		auto right = _actor.begin() + 1;
		base::collider leftCollider;
		base::collider rightCollider;
		for (; right != _actor.end();++right,++left)
		{
			leftCollider = (*left)->getCollider();
			rightCollider = (*right)->getCollider();
			if ((*left)->getPosition().x + leftCollider.right > (*right)->getPosition().x - rightCollider.left
				&& (*left)->getPosition().x - leftCollider.left < (*right)->getPosition().x + rightCollider.right)
			{
				if ((*left)->getPosition().y + leftCollider.up > (*right)->getPosition().y - rightCollider.down
					&& (*left)->getPosition().y - leftCollider.down < (*right)->getPosition().y + rightCollider.up)
				{
					(*left)->onCollision(*right);
					(*right)->onCollision(*left);
				}
			}
		}
	}
	if (!_enemyActor.empty())
	{
		auto left = _enemyActor.begin();
		auto right = _enemyActor.begin() + 1;
		base::collider leftCollider;
		base::collider rightCollider;
		for (; right != _enemyActor.end(); ++right, ++left)
		{
			leftCollider = (*left)->getCollider();
			rightCollider = (*right)->getCollider();
			if ((*left)->getPosition().x + leftCollider.right > (*right)->getPosition().x - rightCollider.left
				&& (*left)->getPosition().x - leftCollider.left < (*right)->getPosition().x + rightCollider.right)
			{
				if ((*left)->getPosition().y + leftCollider.up > (*right)->getPosition().y - rightCollider.down
					&& (*left)->getPosition().y - leftCollider.down < (*right)->getPosition().y + rightCollider.up)
				{
					(*left)->onCollision(*right);
					(*right)->onCollision(*left);
				}
			}
		}
	}
}

core::Renderer::Renderer()
{
	LOG_INFO("Creating window...");
	_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1920, 1080), "Age of War");
	_window->setFramerateLimit(60);
}
