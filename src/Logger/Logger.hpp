#pragma once
#include "LoggerFunctions.hpp"
#include <iostream>


template<typename U, typename ...Ts>
void varPrint(U&& param, Ts&&...args)
{
	std::cout << param;
	varPrint(std::forward<Ts>(args)...);
}

template<typename U,typename F, typename ...Ts>
void infoPrint(U&& file, F&& line, Ts&& ... args)
{
	std::cout << " at : " << file<<" line : "<<line<<"\n";
	varPrint(std::forward<Ts>(args)...);
}

template<typename ...Ts>
void LOG_INFO(Ts&& ... args)
{
	std::cout << "[LOG_INFO]";
	disTime();
	
	infoPrint(std::forward<Ts>(args)...);
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


#define LOG_INFO(...) { LOG_INFO(__FILE__, __LINE__, __VA_ARGS__); }
#define LOG_WARNING(...) { LOG_WARNING(__FILE__, __LINE__, __VA_ARGS__); }
#define LOG_ERROR(...) { LOG_ERROR(__FILE__, __LINE__,  __VA_ARGS__); }