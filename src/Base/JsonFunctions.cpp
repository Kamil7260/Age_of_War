#include "JsonFunctions.hpp"
#include "../Core/Application.hpp"
#include "../Logger/Logger.hpp"

const base::jsonInfo base::loadUnitFromJson(const unsigned int type, const std::string& age)
{
	auto& info = core::Application::getInstance().getMobInfo();

	auto obj = info[age];

	base::jsonInfo mob;
	mob.name = age + "T" + std::to_string(type);
	if (obj == nullptr)
	{
		LOG_ERROR("age : ", age, " does not exist in json");
		return mob;
	}
	obj = obj["T" + std::to_string(type)];
	if (obj == nullptr)
	{
		LOG_ERROR("age : ", age, " -> ", type, " does not exist in json");
		return mob;
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
		LOG_ERROR("age : ", age, " index -> ", type, "!= collider");
		return mob;
	}
	auto hp = obj["hp"];
	if (hp == nullptr) {

		LOG_ERROR("age : ", age, " index -> ", type, "!= hp");
		return mob;
	}
	mob.hp = hp;
	auto minAttack = obj["minDMG"];
	auto maxAttack = obj["maxDMG"];
	auto speedAttack = obj["speedAttack"];
	if (maxAttack == nullptr || minAttack == nullptr || speedAttack == nullptr)
	{
		LOG_ERROR("age : ", age, " index -> ", type, "!= attack");
		return mob;
	}
	mob.minDMG = minAttack;
	mob.maxDMG = maxAttack;
	mob.speedAttack = speedAttack;
	auto animationSpeed = obj["animationSpeed"];
	if (animationSpeed == nullptr)
	{
		LOG_ERROR("age : ", age, " index -> ", type, "!= animationSpeed");
		return mob;
	}
	mob.animationSpeed = animationSpeed;
	auto speedMove = obj["speedMove"];
	if (speedMove == nullptr)
	{
		LOG_ERROR("age : ", age, " index -> ", type, "!= speedMove");
		return mob;
	}
	mob.speedMove = speedMove;
	auto scale = obj["scale"];
	if (scale == nullptr)
	{
		LOG_ERROR("age : ", age, " index -> ", type, "!= scale");
		return mob;
	}
	mob.scale = sf::Vector2f(scale.at(0), scale.at(1));
	auto spawnTime = obj["spawnTime"];
	if (spawnTime == nullptr)
	{
		LOG_ERROR("age : ", age, " index -> ", type, "!= spawnTime");
		return mob;
	}
	mob.spawnTime = spawnTime;
	if (type == 1) {
		auto range = obj["range"];
		if (range == nullptr)
		{
			LOG_ERROR("age : ", age, " index -> ", type, "!= range");
			return mob;
		}
		mob.range = range;
	}
	auto income = obj["income"];
	if (income == nullptr)
	{
		LOG_ERROR("age : ", age, " index -> ", type, "!= income");
		return mob;
	}
	mob.income = income;
	auto price = obj["price"];
	if (price == nullptr)
	{
		LOG_ERROR("age : ", age, " index -> ", type, "!= price");
		return mob;
	}
	mob.price = price;
	return mob;
}

const base::jsonInfo base::loadCannonFromJson(const std::string& age, const unsigned int type)
{
	auto& info = core::Application::getInstance().getCannonInfo();

	auto obj = info[age];
	base::jsonInfo mob;
	mob.name = age + "C" + std::to_string(type);

	if (obj == nullptr)
	{
		LOG_ERROR(mob.name, " does not exist in json");
		return mob;
	}
	obj = obj["C" + std::to_string(type)];

	if (obj == nullptr)
	{
		LOG_ERROR(mob.name, " does not exist in json");
		return mob;
	}

	auto animationSpeed = obj["animationSpeed"];
	if (animationSpeed == nullptr) {
		LOG_ERROR("Can not get animation speed for cannon : ", mob.name);
		return mob;
	}
	mob.animationSpeed = animationSpeed;
	auto maxDMG = obj["maxDMG"];
	if (maxDMG == nullptr) {
		LOG_ERROR("Can not get max dmg for cannon : ", mob.name);
		return mob;
	}
	mob.maxDMG = maxDMG;

	auto minDMG = obj["minDMG"];
	if (minDMG == nullptr) {
		LOG_ERROR("Can not get min dmg for cannon : ", mob.name);
		return mob;
	}
	mob.minDMG = minDMG;

	auto range = obj["range"];
	if (range == nullptr) {
		LOG_ERROR("Can not get range for cannon : ", mob.name);
		return mob;
	}
	mob.range = range;

	auto reload = obj["reloadTime"];
	if (range == nullptr) {
		LOG_ERROR("Can not get reloadTime for cannon : ", mob.name);
		return mob;
	}
	mob.reloadTime = reload;

	auto bulletSpeed = obj["bulletSpeed"];
	if (range == nullptr) {
		LOG_ERROR("Can not get bulletSpeed for cannon : ", mob.name);
		return mob;
	}
	mob.bulletSpeed = bulletSpeed;
	auto price = obj["price"];
	if (price == nullptr)
	{
		LOG_ERROR("age : ", age, " index -> ", type, "!= price");
		return mob;
	}
	mob.price = price;
	return mob;
}
