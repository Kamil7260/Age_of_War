#include "ClipManager.hpp"

bool core::ClipManager::addClip(std::unique_ptr<base::Clip> clip, const std::string& name)
{
	auto it = _container.insert(std::make_pair(name, std::move(clip)));
	if (!it.second)
	{
		LOG_ERROR("Clip with name : ", name, " already exist");
		return false;
	}
	return true;
}

void core::ClipManager::removeClip(const std::string& name)
{
	auto it = _container.find(name);
	if (it == _container.end())
	{
		LOG_ERROR("tried to delete clip with name : ", name, "but it does not exist");
	}
	else {
		_container.erase(it);
	}
}

const std::unique_ptr<base::Clip>& core::ClipManager::getClip(const std::string& name)
{
	auto it = _container.find(name);
	if (it == _container.end())
	{
		LOG_ERROR("Clip with name : ", name, " does not exist");
		return std::unique_ptr<base::Clip>(nullptr);
	}
	return it->second;
}
