#pragma once

#include "Ikon.hpp"

class QueueManager : public base::Actor {
public:
	QueueManager(int space = 80);
	QueueManager(const QueueManager&) = delete;
	QueueManager(QueueManager&&) noexcept;

	QueueManager& operator=(const QueueManager&) = delete;
	QueueManager& operator=(QueueManager&&) noexcept;

	virtual void addIkon(Ikon& ikon,const std::function<void()>& onEnd);

	virtual void onUpdate() override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:

	virtual void repos();

	std::vector<std::pair<Ikon,std::function<void()>>> _queueContainer;
	bool _isEnd;
	int _space;
};