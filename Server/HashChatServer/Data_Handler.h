#pragma once
#include "stdafx.h"
#define TABLE_SIZE 10
#define FILE_NAME "UDATA"
#define KEY 13
class Data_Handler
{
public:
	Data_Handler();
	~Data_Handler();
	int ConnectionCounter;
	SOCKET ListenSocket;
	SOCKET TempSocket;

	struct _sData {
			int Type;
			std::string fU;
			std::string fP;
			std::string tU;
			char* Buffer = new char[512];
	};


	struct UserData {
		int Logged;
		std::string userName;
		std::string passWord;
		SOCKET s[2] = { NULL };
		UserData* _uData;
	};

	UserData* uDH[TABLE_SIZE];


	static char* serialize(_sData*, int);
	static _sData* deserialize(char*, size_t);
};

