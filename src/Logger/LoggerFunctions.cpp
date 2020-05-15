#define _CRT_SECURE_NO_WARNINGS

#include "LoggerFunctions.hpp"
#include <iostream>
#include <ctime>


void varPrint()
{
	std::cout << "\n\n";
	return;
}


void disTime()
{
	const time_t tme = time(nullptr);
	char _timeBuffer[16];
	tm* obj = gmtime(&tme);
	obj = localtime(&tme);

	strftime(_timeBuffer, sizeof(_timeBuffer), "[%H:%M:%S] ", obj);
	std::cout << _timeBuffer;
}