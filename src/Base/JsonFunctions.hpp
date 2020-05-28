#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Actor.hpp"
namespace base {
		struct jsonInfo {
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
	

const jsonInfo loadUnitFromJson(const unsigned int type, const std::string& age);

const jsonInfo loadCannonFromJson(const std::string& age,const unsigned int type);
}