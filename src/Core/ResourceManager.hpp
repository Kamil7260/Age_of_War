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
			std::string _path;
			std::shared_ptr<T> _data;
		};

		std::vector<std::shared_ptr<container>> _container;
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
			_container.push_back(std::make_shared<container>());
			auto& k = _container.back();
			k->_path = path;
			k->_data = std::move(temp);
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
			_container.push_back(std::make_shared<container>());
			auto& k = _container.back();
			k->_path = path;
			k->_data = std::move(temp);
		}
		return true;
	}

	template<typename T>
	inline const std::shared_ptr<T> ResourceManager<T>::get(const std::string& path)
	{
		for (auto k = _container.begin(); k != _container.end(); ++k)
		{
			if ((*k)->_path == path)
			{
				return (*k)->_data;
			}
		}
		LOG_WARNING("Tried to get asset that does not exist with path = ", path);
		return std::shared_ptr<T>(nullptr);
	}

	template<typename T>
	inline bool ResourceManager<T>::remove(const std::string& path)
	{
		for (auto it = _container.begin(); it != _container.end(); ++it)
		{
			if ((*it)->_path == path)
			{
				if(it != _container.end()-1)
					std::iter_swap(it,_container.end() - 1);
				_container.pop_back();
				return true;
			}
		}
		LOG_WARNING("Tried to remove asset that does not exist with path = ", path);
		return false;
	}
}