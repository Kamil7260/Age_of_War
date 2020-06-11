#include "UpgradeButton.hpp"
#include "../Core/Renderer.hpp"

UpgradeButton::UpgradeButton(const base::collider& collider)
	:Button(collider), m_lvl(0)
{
}

void UpgradeButton::increaseLvl()
{
	++m_lvl;
	if (m_lvl < m_levels.size())
	{
		m_sprite.setTexture(*m_levels[m_lvl]);
	}
	else {
		m_isActive = false;
		core::Renderer::getInstance().clearNoActive();
	}
}

void UpgradeButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite,states);
	if(m_drawBox)
		m_infoBox.draw(target, states);
}
