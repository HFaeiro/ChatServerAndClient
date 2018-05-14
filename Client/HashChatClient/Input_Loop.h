#pragma once
#include "Hash_Handler.h"
class Input_Loop
{


public:
	Input_Loop();
	~Input_Loop();

	static int Main_Loop(Data_Handler*);
	static std::string parse(char*, char**);
	static int Recv_Loop(Data_Handler*);
	static void Enter_Uinfo(char*, std::string*, std::string*);




public:



};

