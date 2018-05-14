#include "Input_Loop.h"



Input_Loop::Input_Loop()
{
}


Input_Loop::~Input_Loop()
{
}

int Input_Loop::Main_Loop(Data_Handler * _Handle)
{
	Data_Handler::_sData* sData = new Data_Handler::_sData();





	int i = 1;
	std::cout << "\nCommands: /1 = login, /2 = Create Account\n";
	std::cout << "ex.. /1 Username Password\n";
	std::string name;
	std::string pass;
	int ssize = NULL;
	int len = NULL;
	int succ = NULL;
	BOOL Logged = NULL;
	for (;;)
	{
		char* Buffer = (char*)malloc(1024);
		name = "";
		pass = "";
		std::cout << ": ";
		std::cin.clear();
		std::cin.getline(Buffer, BUFFER_SIZE);

		switch (Buffer[0])
		{
		case '/':
			if (!Logged) {
				switch (Buffer[1])
				{
				case '1': // Login
					Enter_Uinfo(Buffer, &name, &pass);
					/////////////Send data to server.
					sData->Type = 1;
					sData->fU = name;
					sData->fP = pass;
					len = sizeof(*sData);
					Buffer = _Handle->serialize(sData, 1024);
					send(_Handle->s[0], Buffer, len, 0);
					recv(_Handle->s[0], (char*)&succ, sizeof(int), 0);
					if (succ)
					{
						succ = NULL;
						sData->Type = 0;
						Buffer = _Handle->serialize(sData, 1024);
						_Handle->s[1] = socket(_Handle->ptr->ai_family, _Handle->ptr->ai_socktype, _Handle->ptr->ai_protocol); // Initialize new socket with server info. 
						connect(_Handle->s[1], _Handle->ptr->ai_addr, (int)_Handle->ptr->ai_addrlen); //reconnect so the server immediately has a chat socket for this client. 
						send(_Handle->s[1], Buffer, len, 0);
						recv(_Handle->s[1], (char*)&succ, sizeof(int), 0);
						if (succ) {
							CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Recv_Loop, (LPVOID)_Handle, NULL, NULL);
							std::cout << "Logged in!\n";
							Logged = TRUE;
							std::cout << "\nCommands: /c = Chat \n";
							std::cout << "ex.. /1 Username Message\n";
						}
						else
							std::cout << "There's been a server side error, please try again\n";
						break;
					}
					std::cout << "Invalid Username or Password\n";
					break;
				case '2'://Create Account
					Enter_Uinfo(Buffer, &name, &pass);
					/////////////Send data to server.
					sData->Type = 2;
					sData->fU = name;
					sData->fP = pass;
					len = sizeof(*sData);
					Buffer = _Handle->serialize(sData, 1024);
					send(_Handle->s[0], Buffer, len, 0);
					recv(_Handle->s[0], (char*)&succ, sizeof(int), 0);
					if (!succ)
					{
						std::cout << "Account Successfully Created!\n";
						break;
					}
					std::cout << "Username is taken\n";
					break;
				default:
					std::cout << "Invalid Command\n";
					break;
				}
				break;
			}

			switch (Buffer[1])
			{
			case 'c':
			case 'C':
				len = sizeof(*sData);
				sData->Type = 3;
				sData->tU = parse(&Buffer[3], &Buffer);
				sData->Buffer = Buffer;
				Buffer = _Handle->serialize(sData, 1024);
				send(_Handle->s[0], Buffer, len, 0);
				break;
			default:
				std::cout << "Invalid Command\n";
				break;
			}
			break;

		default:
			if (Logged)
			{
				if (sData->tU != "")
				{
					sData->Type = 3;
					//sData->tU = parse(&Buffer[3], &Buffer);
					sData->Buffer = Buffer;
					Buffer = _Handle->serialize(sData, 1024);
					send(_Handle->s[1], Buffer, len, 0);
				}
				else
					std::cout << "Invalid Command\n";
			}
			else
				std::cout << "Invalid Command\n";
			break;
		}
	}
	return 0;
}

int Input_Loop::Recv_Loop(Data_Handler * _Handle)
{

	Data_Handler::_sData* rData = new Data_Handler::_sData();


	SOCKET sRecv = _Handle->s[1];
	ULONG ul = 1;
	ioctlsocket(sRecv, FIONBIO, &ul);
	char* Buffer = (char*)malloc(1024);
	for (;;)
	{
		fd_set recvChat;
		timeval conTime;
		conTime.tv_sec = 0;
		conTime.tv_usec = 10000; //waits /10000 microseconds /10 milliseconds /.01 seconds
		FD_ZERO(&recvChat); //sets struct to NULL
		FD_SET(sRecv, &recvChat); // adds unochat to struct, also adds +1 to struct count.
		int sel = select(NULL, &recvChat, NULL, NULL, &conTime); //checks the status of the sockets. 
		ULONG bytesAvail = NULL;
		int retval = NULL;

		if (sel > 0)
		{
			if (FD_ISSET(sRecv, &recvChat)) // if data is waiting to be received from server.
			{
				ioctlsocket(sRecv, FIONREAD, &bytesAvail); //how many bytes are to be received.
				int bAvail = bytesAvail;
				if (bAvail) { //if there are bytes to be received. 
					retval = recv(sRecv, Buffer, bAvail, 0); //receive those bytes.
					if (retval == -1) //if recv call had an error return.
						return retval;
					rData = _Handle->deserialize(Buffer, bAvail);

					//rData->Buffer[bAvail - 12] = '\0';
					std::cout << rData->fU;

					printf(": %s\n", rData->Buffer);
					if (!rData->Type)
						return rData->Type;
					Buffer = (char*)malloc(BUFFER_SIZE);
				}
			}
		}
	}


}

std::string Input_Loop::parse(char* Buffer, char** ptr)
{
	std::string buffer;
	int i = NULL;

	for (i; (Buffer[i] != ' ') && (Buffer[i] != '\0'); i++)
	{
		buffer += Buffer[i];
		//ptr = Buffer[i];
	}
	char* j = new char();
	*ptr = j;
	*ptr = &Buffer[i + 1];


	return buffer;
}
void Input_Loop::Enter_Uinfo(char *Buffer, std::string *name, std::string *pass)
{

	int ssize = NULL;

	if (Buffer[3] == NULL)
	{
		std::cout << "Enter Username: ";
		std::cin >> *name;
		std::cout << "Enter Password: ";
		std::cin >> *pass;
		std::cin.clear();
		std::cin.ignore();
	}
	else {
		for (int i = 3;(Buffer[i] != ' ') && (Buffer[i] != '\0'); i++)
			*name += Buffer[i];
		ssize = (*name).length() + 4;
		if (strlen(Buffer) < ssize)
		{
			std::cout << "Enter Password: ";
			std::cin >> *pass;
			std::cin.clear();
			std::cin.ignore();
			return;
		}
		for (int i = ssize; Buffer[i] != '\0'; i++)
			*pass += Buffer[i];
	}
}