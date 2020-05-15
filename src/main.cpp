#include "Core/Application.hpp"

int main()
{
	LOG_INFO("Creating application");
	auto& k = core::Application::getInstance();

	k.run();

	LOG_INFO("Closing application");
	return 0;
}