#pragma once

#include "Button.hpp"

class UpgradeButton : public Button
{
public:

	UpgradeButton(const base::collider& collider = { 1.f,1.f,1.f,1.f });
	UpgradeButton(const UpgradeButton&) = default;
	UpgradeButton(UpgradeButton&&) = default;

	UpgradeButton& operator=(const UpgradeButton&) = default;
	UpgradeButton& operator=(UpgradeButton&&) = default;

	virtual void increaseLvl();

	virtual void addLevel(const std::shared_ptr<sf::Texture>& tex) {
		m_levels.push_back(tex);
		m_sprite.setTexture(*m_levels[m_lvl]);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
protected:
	std::vector<std::shared_ptr<sf::Texture>> m_levels;
	unsigned int m_lvl;
	
};