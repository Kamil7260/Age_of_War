#pragma once
#include <vector>

#include "../Base/Actor.hpp"
#include <sstream>
class Info : public base::Actor {
public:
	Info() = default;
	Info(const Info&) = default;
	Info(Info&&) = default;

	Info& operator=(const Info&) = default;
	Info& operator=(Info&&) = default;

	virtual void setPosition(const sf::Vector2f& pos) override;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void onUpdate() override;

	virtual void setString(const std::string& info);

	virtual void setScale(const sf::Vector2f& sca) override;

	virtual void setFont(const sf::Font& font);
	virtual void setColor(const sf::Color& color);

	virtual void setTexture(const sf::Texture& tex);

	virtual void setCharacterSize(const unsigned int size);

	template<typename T>
	void addValue(T* target, const std::string& description);

	void refresh();
	void clear();
protected:

	class type {
	public:
		type() = default;
		type(const type&) = default;
		type(type&&) = default;
		type& operator=(const type&) = default;
		type& operator=(type&&) = default;
		virtual const std::string getString() = 0;
	};


	template<typename T>
	class namedType : public type {
	public:
		namedType(T* target)
			:m_target(target)
		{}
		namedType(const namedType&) = default;
		namedType(namedType&&) = default;
		namedType& operator=(const namedType&) = default;
		namedType& operator=(namedType&&) = default;
		const std::string getString() override {
			std::stringstream stream;
			stream << *m_target;
			std::string type;
			stream >> type;
			return type;
		}

	private:
		T* m_target;
	};

	std::vector<std::pair<std::string, std::unique_ptr<type>>> m_container;

	sf::Sprite m_sprite;
	sf::Text m_text;

};

template<typename T>
inline void Info::addValue(T* target,const std::string& description)
{
	auto p = std::make_unique<namedType<T>>(target);
	m_container.push_back(std::make_pair(description, std::move(p)));
}
