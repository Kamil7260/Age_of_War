#include "JsonFunctions.hpp"
#include "../Core/Application.hpp"
#include "../Logger/Logger.hpp"

const base::unitInfo base::loadUnitFromJson(const unsigned int type, const std::string& age)
{
	auto& info = core::Application::getInstance().getMobInfo();
	base::unitInfo mob;
	try {
		auto obj = info[age];
		bool shouldStopApp = false;
		mob.name = age + "T" + std::to_string(type);
		if (obj == nullptr)
		{
			shouldStopApp = true;
			LOG_ERROR("age : ", age, " does not exist in json");
		}
		obj = obj["T" + std::to_string(type)];
		if (obj == nullptr)
		{
			shouldStopApp = true;
			LOG_ERROR("age : ", age, " -> ", type, " does not exist in json");
		}
		auto collider = obj["collider"];
		base::collider c;
		if (collider != nullptr)
		{
			c.up = collider.at(0);
			c.down = collider.at(1);
			c.left = collider.at(2);
			c.right = collider.at(3);
			mob.collider = c;
		}
		else {
			shouldStopApp = true;
			LOG_ERROR("age : ", age, " index -> ", type, "!= collider");
		}
		auto hp = obj["hp"];
		if (hp == nullptr) {
			shouldStopApp = true;
			LOG_ERROR("age : ", age, " index -> ", type, "!= hp");
		}
		mob.hp = hp;
		auto minAttack = obj["minDMG"];
		auto maxAttack = obj["maxDMG"];
		auto speedAttack = obj["speedAttack"];
		if (maxAttack == nullptr || minAttack == nullptr || speedAttack == nullptr)
		{
			shouldStopApp = true;
			LOG_ERROR("age : ", age, " index -> ", type, "!= attack");
		}
		mob.minDMG = minAttack;
		mob.maxDMG = maxAttack;
		mob.speedAttack = speedAttack;
		auto animationSpeed = obj["animationSpeed"];
		if (animationSpeed == nullptr)
		{
			shouldStopApp = true;
			LOG_ERROR("age : ", age, " index -> ", type, "!= animationSpeed");
		}
		mob.animationSpeed = animationSpeed;
		auto speedMove = obj["speedMove"];
		if (speedMove == nullptr)
		{
			shouldStopApp = true;
			LOG_ERROR("age : ", age, " index -> ", type, "!= speedMove");
		}
		mob.speedMove = speedMove;
		auto scale = obj["scale"];
		if (scale == nullptr)
		{
			shouldStopApp = true;
			LOG_ERROR("age : ", age, " index -> ", type, "!= scale");
		}
		mob.scale = sf::Vector2f(scale.at(0), scale.at(1));
		auto spawnTime = obj["spawnTime"];
		if (spawnTime == nullptr)
		{
			shouldStopApp = true;
			LOG_ERROR("age : ", age, " index -> ", type, "!= spawnTime");
		}
		mob.spawnTime = spawnTime;
		if (type == 1) {
			auto range = obj["range"];
			if (range == nullptr)
			{
				shouldStopApp = true;
				LOG_ERROR("age : ", age, " index -> ", type, "!= range");
			}
			mob.range = range;
		}
		auto income = obj["income"];
		if (income == nullptr)
		{
			shouldStopApp = true;
			LOG_ERROR("age : ", age, " index -> ", type, "!= income");
		}
		mob.income = income;
		auto price = obj["price"];
		if (price == nullptr)
		{
			shouldStopApp = true;
			LOG_ERROR("age : ", age, " index -> ", type, "!= price");
		}
		mob.price = price;
		auto displayName = obj["name"];
		if (displayName == nullptr)
		{
			shouldStopApp = true;
			LOG_ERROR("age : ", age, " index -> ", type, "!= displayName");
		}
		mob.displayName = displayName;

		if (shouldStopApp)
		{
			core::Application::getInstance().freezeScreen();
		}
	}
	catch (...)
	{
		LOG_ERROR("Exception at UnitLoader");
		core::Application::getInstance().freezeScreen();
	}
	return mob;
}

const base::cannonInfo base::loadCannonFromJson(const std::string& age, const unsigned int type)
{
	auto& info = core::Application::getInstance().getCannonInfo();
	
	base::cannonInfo mob;
	try {
		bool shouldStopApp = false;
		auto obj = info[age];
		mob.name = age + "C" + std::to_string(type);

		if (obj == nullptr)
		{
			shouldStopApp = true;
			LOG_ERROR(mob.name, " does not exist in json");
		}
		obj = obj["C" + std::to_string(type)];

		if (obj == nullptr)
		{
			shouldStopApp = true;
			LOG_ERROR(mob.name, " does not exist in json");
		}

		auto animationSpeed = obj["animationSpeed"];
		if (animationSpeed == nullptr) {
			LOG_ERROR("Can not get animation speed for cannon : ", mob.name);
			shouldStopApp = true;
		}
		mob.animationSpeed = animationSpeed;
		auto maxDMG = obj["maxDMG"];
		if (maxDMG == nullptr) {
			LOG_ERROR("Can not get max dmg for cannon : ", mob.name);
			shouldStopApp = true;
		}
		mob.maxDMG = maxDMG;

		auto minDMG = obj["minDMG"];
		if (minDMG == nullptr) {
			LOG_ERROR("Can not get min dmg for cannon : ", mob.name);
			shouldStopApp = true;
		}
		mob.minDMG = minDMG;

		auto range = obj["range"];
		if (range == nullptr) {
			LOG_ERROR("Can not get range for cannon : ", mob.name);
			shouldStopApp = true;
		}
		mob.range = range;

		auto reload = obj["reloadTime"];
		if (range == nullptr) {
			LOG_ERROR("Can not get reloadTime for cannon : ", mob.name);
			shouldStopApp = true;
		}
		mob.reloadTime = reload;

		auto bulletSpeed = obj["bulletSpeed"];
		if (range == nullptr) {
			LOG_ERROR("Can not get bulletSpeed for cannon : ", mob.name);
			shouldStopApp = true;
		}
		mob.bulletSpeed = bulletSpeed;
		auto price = obj["price"];
		if (price == nullptr)
		{
			LOG_ERROR("age : ", age, " index -> ", type, "!= price");
			shouldStopApp = true;
		}
		mob.price = price;
		auto displayName = obj["name"];
		if (displayName == nullptr)
		{
			LOG_ERROR("age : ", age, " index -> ", type, "!= displayName");
			shouldStopApp = true;
		}
		mob.displayName = displayName;

		auto bulletSpawn = obj["bulletSpawn"];
		if (bulletSpawn == nullptr)
		{
			LOG_ERROR("age : ", age, " index -> ", type, "!= bulletSpawn");
			shouldStopApp = true;
		}
		mob.bulletPosition = bulletSpawn;

		auto long_range = obj["long_range"];
		if (long_range == nullptr) {
			LOG_ERROR("age : ", age, " index -> ", type, "!= long_range");
			shouldStopApp = true;
		}
		mob.long_range = long_range;

		auto firespeed = obj["firespeed"];
		if (firespeed == nullptr) {
			LOG_ERROR("age : ", age, " index -> ", type, "!= firespeed");
			shouldStopApp = true;
		}
		mob.fireSpeed = firespeed;
		if (shouldStopApp)
		{
			core::Application::getInstance().freezeScreen();
		}
	}
	catch (...)
	{
		LOG_ERROR("Exception at CannonLoader");
		core::Application::getInstance().freezeScreen();
	}
	return mob;
}
