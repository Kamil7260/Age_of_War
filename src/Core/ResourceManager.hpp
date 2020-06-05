#pragma once
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "../Logger/Logger.hpp"

namespace core {

	template<typename T>
	class ResourceManager final {
	public:
		bool loadFromFile(const std::string& path);
		bool loadFromFile(const std::string& path,const sf::IntRect& rect);
		const std::shared_ptr<T> get(const std::string& path);

		bool remove(const std::string& path);

		static ResourceManager& getInstance() {
			static ResourceManager manager;
			return manager;
		}

		ResourceManager(ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
		~ResourceManager() = default;
	private:
		ResourceManager() = default;

		struct container {
			std::string m_path;
			std::shared_ptr<T> m_data;
		};

		std::vector<std::shared_ptr<container>> m_container;
	};

	template<typename T>
	inline bool ResourceManager<T>::loadFromFile(const std::string& path)
	{
		std::shared_ptr<T> temp = std::make_shared<T>();
		if (!temp->loadFromFile(path))
		{
			LOG_ERROR("Can not open file with path = ", path);
			return false;
		}
		else {
			m_container.push_back(std::make_shared<container>());
			auto& k = m_container.back();
			k->m_path = path;
			k->m_data = std::move(temp);
		}
		return true;
	}

	template<typename T>
	inline bool ResourceManager<T>::loadFromFile(const std::string& path, const sf::IntRect& rect)
	{
		std::shared_ptr<T> temp = std::make_shared<T>();
		if (!temp->loadFromFile(path,rect))
		{
			LOG_ERROR("Can not open file with path = ", path);
			return false;
		}
		else {
			m_container.push_back(std::make_shared<container>());
			auto& k = m_container.back();
			k->m_path = path;
			k->m_data = std::move(temp);
		}
		return true;
	}

	template<typename T>
	inline const std::shared_ptr<T> ResourceManager<T>::get(const std::string& path)
	{
		for (auto k = m_container.begin(); k != m_container.end(); ++k)
		{
			if ((*k)->m_path == path)
			{
				return (*k)->m_data;
			}
		}
		LOG_WARNING("Tried to get asset that does not exist with path = ", path);
		return std::shared_ptr<T>(nullptr);
	}

	template<typename T>
	inline bool ResourceManager<T>::remove(const std::string& path)
	{
		for (auto it = m_container.begin(); it != m_container.end(); ++it)
		{
			if ((*it)->_path == path)
			{
				if(it != m_container.end()-1)
					std::iter_swap(it,m_container.end() - 1);
				m_container.pop_back();
				return true;
			}
		}
		LOG_WARNING("Tried to remove asset that does not exist with path = ", path);
		return false;
	}
}