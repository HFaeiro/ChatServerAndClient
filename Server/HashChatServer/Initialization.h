#pragma once
#include "Connection_Handler.h"

class Initialization
{
public:
	Initialization();
	~Initialization();
	int Initialize(Data_Handler*);
	static void Static_Listen(Data_Handler*);
	//////////////////////////////////////////////////////
	struct SETTINGS
	{
		int port;
		char URL[256] = "127.0.0.1";
	}data;


private:
	WSADATA wsaData;
	struct addrinfo *result, *ptr, hints;



};
//Needed for static function.
static class sInitialization
{
public:
	Connection_Handler *_cHandler;
}sInit;
