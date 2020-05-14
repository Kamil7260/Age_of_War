#include "LoggerFunctions.hpp"
#include <iostream>

template<typename U, typename ...Ts>
void varPrint(U&& param, Ts&&...args)
{
	std::cout << param;
	varPrint(std::forward<Ts>(args)...);
}

template<typename U, typename ...Ts>
void LOG_INFO(U&& param, Ts&& ... args)
{
	std::cout << "[LOG_INFO]";
	disTime();
	std::cout << param;
	varPrint(std::forward<Ts>(args)...);
}

template<typename U, typename ...Ts>
void LOG_WARNING(U&& param,  Ts&& ... args)
{
	std::cout << "[LOG_WARNING]";
	disTime();
	std::cout << param;
	varPrint(std::forward<Ts>(args)...);
}

template<typename U, typename ...Ts>
void LOG_ERROR(U&& param,  Ts&& ... args)
{
	std::cout << "[LOG_ERROR]";
	disTime();
	std::cout << param;
	varPrint(std::forward<Ts>(args)...);
}