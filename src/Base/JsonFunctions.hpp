#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Actor.hpp"
namespace base {
		struct unitInfo {
			std::string displayName = "none";
			std::string name = "";
			base::collider collider = { 1.f,1.f,1.f,1.f };
			int hp = 20;
			int maxDMG = 3;
			int minDMG = 1;
			float speedAttack = 1.f;
			float animationSpeed = 0.f;
			float speedMove = 1.f;
			sf::Vector2f scale = { 1.f,1.f };
			int range = 200;
			float spawnTime = 20.f;
			float reloadTime = 20.f;
			float bulletSpeed = 1.f;
			int income = 10;
			int price = 10;
	};
		struct cannonInfo {
			std::string displayName = "none";
			std::string name = "";
			float bulletPosition = 0;
			int maxDMG = 3;
			int minDMG = 1;
			float speedAttack = 1.f;
			float animationSpeed = 0.f;
			sf::Vector2f scale = { 1.f,1.f };
			int range = 200;
			float reloadTime = 20.f;
			float bulletSpeed = 1.f;
			int price = 10;
			bool long_range = false;
			float fireSpeed = 2.f;
		};
	

const unitInfo loadUnitFromJson(const unsigned int type, const std::string& age);

const cannonInfo loadCannonFromJson(const std::string& age,const unsigned int type);
}