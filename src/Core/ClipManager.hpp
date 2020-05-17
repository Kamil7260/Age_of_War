#pragma once
#include <map>
#include <string>
#include <memory>
#include "../Logger/Logger.hpp"

namespace core {
	template<typename T>
	class ClipManager final {
	public:

		ClipManager() = default;
		ClipManager(ClipManager&) = delete;
		ClipManager(ClipManager&&) = delete;
		ClipManager& operator=(ClipManager&) = delete;
		ClipManager& operator=(ClipManager&&) = delete;

		bool addClip(const std::shared_ptr<T>& clip, const std::string& name);
		void removeClip(const std::string& name);

		const std::shared_ptr<T>& getClip(const std::string& name);

	private:
		std::map<std::string, std::shared_ptr<T>> _container;
	};


	template<typename T>
	bool ClipManager<T>::addClip(const std::shared_ptr<T>& clip, const std::string& name)
	{
		auto it = _container.insert(std::make_pair(name, clip));
		if (!it.second)
		{
			LOG_ERROR("Clip with name : ", name, " already exist");
			return false;
		}
		return true;
	}

	template<typename T>
	void ClipManager<T>::removeClip(const std::string& name)
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

	template<typename T>
	const std::shared_ptr<T>& ClipManager<T>::getClip(const std::string& name)
	{
		auto it = _container.find(name);
		if (it == _container.end())
		{
			LOG_ERROR("Clip with name : ", name, " does not exist");
			return nullptr;
		}
		return it->second;
	}

}