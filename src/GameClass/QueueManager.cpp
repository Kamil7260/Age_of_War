#include "QueueManager.hpp"

QueueManager::QueueManager(int space)
	:m_space(space),m_isEnd(false)
{
	m_tag = "QueueManager";
}

void QueueManager::addIkon(Ikon& ikon, const std::function<void()>& onEnd)
{
	ikon.setCallBackOnEnd([&]()->void {
		m_isEnd = true;
	});
	m_queueContainer.push_back(std::make_pair(std::move(ikon),onEnd));
	repos();
}

void QueueManager::onUpdate()
{
	if (!m_queueContainer.empty())
	{
		m_queueContainer[0].first.onUpdate();
		if (m_isEnd == true)
		{
			m_queueContainer[0].second();
			m_queueContainer.erase(m_queueContainer.begin());
			m_isEnd = false;
			repos();
		}
	}
}

void QueueManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto k = m_queueContainer.begin(); k != m_queueContainer.end(); ++k)
	{
		target.draw(k->first);
	}
}

void QueueManager::repos()
{
	int queuePosition = 0;
	for (auto k = m_queueContainer.begin(); k != m_queueContainer.end(); ++k)
	{
		k->first.setPosition(sf::Vector2f(m_position.x + m_space * queuePosition, m_position.y));
		++queuePosition;
	}
}
