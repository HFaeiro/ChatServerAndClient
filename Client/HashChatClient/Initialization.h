#pragma once
#include "Input_Loop.h"
class Initialization
{
public:
	Initialization();
	~Initialization();
	int Initialize(Data_Handler*);



private:
	Input_Loop* iLoop = new Input_Loop();
	WSADATA wsaData;
	int iResult;
	struct addrinfo *result = NULL, hints;
	void Connect(Data_Handler*);


};

