#include "Initialization.h"



#include "Initialization.h"



Initialization::Initialization()
{

	sInit._cHandler = new Connection_Handler;

	result = NULL;
	ptr = NULL;
	data.port = 6420;

}


Initialization::~Initialization()
{
}
int Initialization::Initialize(Data_Handler* _Handle)
{
	int iResult = NULL;
	_Handle->ListenSocket = INVALID_SOCKET;
	


	if ((iResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		printf("WSAStartup Failed: %d\n", iResult);
		return 1;
	}
	printf("WinSock Initialized...\n");

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	std::stringstream ss;
	ss << data.port;
	std::string str = ss.str();

	if ((iResult = getaddrinfo(data.URL, str.c_str(), &hints, &result)) != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	struct in_addr addr;
	((struct sockaddr_in *)(result->ai_addr))->sin_addr.s_addr = htonl(INADDR_ANY);


	if ((_Handle->ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	if ((iResult = bind(_Handle->ListenSocket, result->ai_addr, (int)result->ai_addrlen)) == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(_Handle->ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	if (listen(_Handle->ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(_Handle->ListenSocket);
		WSACleanup();
		return 1;
	}
	printf("Listening....\n");
	//SetWindowText(0, L"Server Listening...");
	HANDLE waitThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Static_Listen, (LPVOID)_Handle, NULL, NULL);
	WaitForSingleObject(waitThread, INFINITE);

	return 0;
}
void Initialization::Static_Listen(Data_Handler* _Handle)
{


	struct sockaddr_in ClientInfo = { 0 };
	int size = sizeof(ClientInfo);
	socklen_t sSize = sizeof(ClientInfo);
	while (TRUE) {
		if ((_Handle->TempSocket = accept(_Handle->ListenSocket, (sockaddr*)&ClientInfo, &size)) != INVALID_SOCKET) {
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)sInit._cHandler->Handle_Client, (LPVOID)_Handle, NULL, NULL);

			
		}
	}
}