#pragma once
#include "Hash_Handler.h"
class Connection_Handler
{
public:
	Connection_Handler();
	~Connection_Handler();
	static int Handle_Client(Data_Handler*);


private:
	struct CurrentClientData {
		std::string clientName;
		SOCKET s[3];
		Data_Handler::_sData* rData;
		Data_Handler* _Handle;
	};
	static void ChatLoop(CurrentClientData*);

};

