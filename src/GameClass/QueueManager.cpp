#include "QueueManager.hpp"

QueueManager::QueueManager(int space)
	:_space(space),_isEnd(false)
{
	_tag = "QueueManager";
}


QueueManager::QueueManager(QueueManager&& source) noexcept
	:_isEnd(source._isEnd), _space(source._space)
{
	for (auto k : source._queueContainer)
	{
		_queueContainer.push_back(k);
	}
	source._queueContainer.clear();
}


QueueManager& QueueManager::operator=(QueueManager&& source) noexcept
{
	for (auto k : source._queueContainer)
	{
		_queueContainer.push_back(k);
	}
	source._queueContainer.clear();
	_isEnd = source._isEnd;
	_space = source._space;
}

void QueueManager::addIkon(Ikon& ikon, const std::function<void()>& onEnd)
{
	ikon.setCallBackOnEnd([&]()->void {
		_isEnd = true;
	});
	_queueContainer.push_back(std::make_pair(std::move(ikon),onEnd));
	repos();
}

void QueueManager::onUpdate()
{
	if (!_queueContainer.empty())
	{
		_queueContainer[0].first.onUpdate();
		if (_isEnd == true)
		{
			_queueContainer[0].second();
			_queueContainer.erase(_queueContainer.begin());
			_isEnd = false;
			repos();
		}
	}
}

void QueueManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto k = _queueContainer.begin(); k != _queueContainer.end(); ++k)
	{
		target.draw(k->first);
	}
}

void QueueManager::repos()
{
	int queuePosition = 0;
	for (auto k = _queueContainer.begin(); k != _queueContainer.end(); ++k)
	{
		k->first.setPosition(sf::Vector2f(_position.x + _space * queuePosition, _position.y));
		++queuePosition;
	}
}
