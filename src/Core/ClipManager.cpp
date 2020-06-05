#include "ClipManager.hpp"

bool core::ClipManager::addClip(std::unique_ptr<base::Clip> clip, const std::string& name)
{
	auto it = m_container.insert(std::make_pair(name, std::move(clip)));
	if (!it.second)
	{
		LOG_ERROR("Clip with name : ", name, " already exist");
		return false;
	}
	return true;
}

void core::ClipManager::removeClip(const std::string& name)
{
	auto it = m_container.find(name);
	if (it == m_container.end())
	{
		LOG_ERROR("tried to delete clip with name : ", name, "but it does not exist");
	}
	else {
		m_container.erase(it);
	}
}

const std::unique_ptr<base::Clip>& core::ClipManager::getClip(const std::string& name)
{
	auto it = m_container.find(name);
	if (it == m_container.end())
	{
		LOG_ERROR("Clip with name : ", name, " does not exist");
		return m_container.begin()->second;
	}
	return it->second;
}
