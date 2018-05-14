#pragma once
#include "stdafx.h"
class Data_Handler
{
public:
	Data_Handler();
	~Data_Handler();


	int ID = NULL;
	SOCKET s[100];	// 0 - Main Connection.
					// 1 - Text Chat.

	struct _sData {
		int Type;
		std::string fU;
		std::string fP;
		std::string tU;
		char* Buffer = new char[512];
	};
	struct addrinfo *ptr = NULL;

	static char* serialize(_sData*, int);
	static _sData* deserialize(char*, size_t);
};


