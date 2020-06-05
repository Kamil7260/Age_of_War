#pragma once
#include <map>
#include <string>
#include <memory>

#include "../Logger/Logger.hpp"
#include "../Base/Clip.hpp"

namespace core {
	class ClipManager final {
	public:

		ClipManager() = default;
		ClipManager(ClipManager&) = delete;
		ClipManager(ClipManager&&) = delete;
		ClipManager& operator=(ClipManager&) = delete;
		ClipManager& operator=(ClipManager&&) = delete;

		~ClipManager() = default;

		bool addClip(std::unique_ptr<base::Clip> clip, const std::string& name);
		void removeClip(const std::string& name);

		const std::unique_ptr<base::Clip>& getClip(const std::string& name);

	private:
		std::map<std::string, std::unique_ptr<base::Clip>> m_container;
	};


}