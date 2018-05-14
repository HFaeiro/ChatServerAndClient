#pragma once
#include "Data_Handler.h"
class Hash_Handler
{
public:
	Hash_Handler();
	~Hash_Handler();


	int HashAdd(Data_Handler*, std::string, std::string);
	//int HashCheck(Data_Handler*, std::string, std::string,SOCKET*, int);
	
	int HashGrab(Data_Handler*, std::string);
	int HashGrab(Data_Handler*, std::string, std::string);
	int HashGrab(Data_Handler*, std::string, SOCKET*);
	int HashGrab(Data_Handler*, std::string, SOCKET);
	
	int HashMod(Data_Handler*, Data_Handler::UserData*);
	void SaveData(Data_Handler*);
	void LoadData(Data_Handler*);
private:
	int HashIndxr(std::string);

};


