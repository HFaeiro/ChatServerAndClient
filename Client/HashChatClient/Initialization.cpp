#include "Initialization.h"



Initialization::Initialization()
{
}


Initialization::~Initialization()
{
}


int Initialization::Initialize(Data_Handler* _Handle)
{

	if ((iResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) { //startup winsock
		printf("WSAStartup Failed: %d\n", iResult);
		return 1;
	}
	printf("WinSock Initialized...\n");

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	std::stringstream ss;
	ss << DEFAULT_PORT;
	std::string str = ss.str();

	if ((iResult = getaddrinfo("127.0.0.1", str.c_str(), &hints, &result)) != 0) { //put all the server info into results.
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}



	_Handle->ptr = result;
	if ((_Handle->s[0] = socket(_Handle->ptr->ai_family, _Handle->ptr->ai_socktype, _Handle->ptr->ai_protocol)) == INVALID_SOCKET) { //initialize socket with server info. 
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	//printf("Socket Connected...\n");

	Connect(_Handle); //connect function




	return 0;

}

void Initialization::Connect(Data_Handler* _Handle)
{
	while (TRUE)
	{
		int iResult = SOCKET_ERROR;


		//constantly attempts to connect to server. // add timeout in the future so this doesn't take up a ton of resources. 
		while (iResult == SOCKET_ERROR)
		{
			iResult = connect(_Handle->s[0], _Handle->ptr->ai_addr, (int)_Handle->ptr->ai_addrlen); //attempts to connect to server. 

		}
		int i = NULL;
		HANDLE waitThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)iLoop->Main_Loop, (LPVOID)_Handle, NULL, NULL);
		WaitForSingleObject(waitThread, INFINITE);
	}
	return;
}