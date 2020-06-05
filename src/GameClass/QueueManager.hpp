#pragma once

#include "Ikon.hpp"

class QueueManager : public base::Actor {
public:
	QueueManager(int space = 80);
	QueueManager(const QueueManager&) = delete;
	QueueManager(QueueManager&&) = default;

	QueueManager& operator=(const QueueManager&) = delete;
	QueueManager& operator=(QueueManager&&) = default;

	virtual ~QueueManager() = default;

	virtual void addIkon(Ikon& ikon,const std::function<void()>& onEnd);

	virtual void onUpdate() override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:

	virtual void repos();

	std::vector<std::pair<Ikon,std::function<void()>>> m_queueContainer;
	bool m_isEnd;
	int m_space;
};