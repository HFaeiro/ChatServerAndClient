#include "Connection_Handler.h"



Connection_Handler::Connection_Handler()
{

}


Connection_Handler::~Connection_Handler()
{
}

int Connection_Handler::Handle_Client(Data_Handler * _Handle) 
{

	CurrentClientData* CCD = new CurrentClientData();
	CCD->_Handle = _Handle;
	CCD->s[1] = NULL;

	CCD->s[0] = _Handle->TempSocket;
	Data_Handler::_sData *rDat = new Data_Handler::_sData();
	Hash_Handler* hh = new Hash_Handler();
	HANDLE cCH = NULL;
	char* Buffer = (char*)malloc(1024);
	int pType = NULL;
	ULONG bAv = 1;
	int succ = NULL;
	ioctlsocket(CCD->s[0], FIONBIO, &bAv);
	while (TRUE) {
		fd_set recvSet;
		timeval conTime;
		conTime.tv_sec = 0;
		conTime.tv_usec = 10000; //waits /10000 microseconds /10 milliseconds /.01 seconds
		FD_ZERO(&recvSet); //sets struct to NULL
		FD_SET(CCD->s[0], &recvSet); // adds unochat to struct, also adds +1 to struct count.
		int sel = select(NULL, &recvSet, NULL, NULL, &conTime); //checks the status of the sockets.
		if (sel > 0) {
			ioctlsocket(CCD->s[0], FIONREAD, &bAv);
			if (!bAv) bAv = 1;
			if (bAv > 1024)
				Buffer = (char*)malloc(bAv);
			int retval = recv(CCD->s[0], Buffer, bAv, 0);// recv Buffer
			if (retval == -1)	break;
			if (retval == 0)	break;
			rDat = _Handle->deserialize(Buffer, retval); //deserialize buffer

			switch (rDat->Type)//TODO:handle create chat socket request && Handle errors//
			{
			case 0: // create chat socket
				succ = hh->HashGrab(_Handle, rDat->fU, CCD->s[0]);
				send(CCD->s[0], (char*)&succ, sizeof(int), 0);
				return succ;
				break;
			case 1:// Login
				succ = hh->HashGrab(_Handle, rDat->fU, rDat->fP);
				send(CCD->s[0], (char*)&succ, sizeof(int), 0);
				break;
			case 2: // Create User
				succ = hh->HashGrab(_Handle, rDat->fU);
				if (!succ)
					hh->HashAdd(_Handle, rDat->fU, rDat->fP);
				send(CCD->s[0], (char*)&succ, sizeof(int), 0);
				break; 
			case 3:// chat
				//if (!hh->HashGrab(_Handle, rDat->fU)) { // check if logged in.
					if (hh->HashGrab(_Handle, rDat->tU)) { // Check if other user is logged in; 
						if (hh->HashGrab(_Handle, rDat->fU, &CCD->s[1])) {// grab chat socket for user.
							if (hh->HashGrab(_Handle, rDat->tU, &CCD->s[2])) {// grab chat socket for user.
								if (cCH)
									WaitForSingleObject(cCH, INFINITE);
								CCD->rData = rDat;
								CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ChatLoop, (LPVOID)CCD, NULL, NULL);//create chat loop thread; //
							}
						}
						//Handle socket is invalid;
					}
					//Handle *user* is not logged in;
			//	}
				//Handle you're not logged in;
				break; 
			default:
				break;

			}

		}
	}
	
	return 0;
}

void Connection_Handler::ChatLoop(CurrentClientData * CCD)
{
	Data_Handler::_sData *rDat = new Data_Handler::_sData();
	*rDat = *CCD->rData;

	ULONG ul = 1;
	SOCKET unoChat = CCD->s[1]; //Transfer client 1 chat socket
	SOCKET dosChat = CCD->s[2];
	ioctlsocket(unoChat, FIONBIO, &ul); // turns socket to non blocking.

	char* Buffer = (char*)malloc(1024);
	Buffer = CCD->_Handle->serialize(rDat, 1024);
	send(dosChat, Buffer, sizeof(*rDat), 0);
	for (;;)
	{
		fd_set duoChat;
		timeval conTime;
		conTime.tv_sec = 0;
		conTime.tv_usec = 10000; //waits /10000 microseconds /10 milliseconds /.01 seconds
		FD_ZERO(&duoChat); //sets struct to NULL
		FD_SET(unoChat, &duoChat); // adds unochat to struct, also adds +1 to struct count.
								   //	FD_SET(dosChat, &duoChat);
		int sel = select(NULL, &duoChat, NULL, NULL, &conTime); //checks the status of the sockets. 
		ULONG bytesAvail = NULL;
		int retval = NULL;

		if (sel > 0)
		{
			if (FD_ISSET(unoChat, &duoChat)) // if data is waiting to be received from Client1
			{
				ioctlsocket(unoChat, FIONREAD, &bytesAvail); //how many bytes are to be received.
				int bAvail = bytesAvail;
				//if (!bAvail) bAvail = 1;  //if there are bytes to be received. if not set it to 1 for error handling. 
				retval = recv(unoChat, Buffer, bAvail, 0); //receive those bytes.
				if (retval == -1 || retval == 0)
					return;
				if (retval) {


					retval = send(dosChat, Buffer, bAvail, 0); // send those bytes to Client2

					if (retval == -1 || retval == 0) {
						rDat = CCD->_Handle->deserialize(Buffer, 12);
						std::stringstream ss;
						ss << "Connection to " << rDat->tU << " Has closed";
						std::string s = ss.str();
						rDat->Buffer = (char*)s.c_str();
						Buffer = CCD->_Handle->serialize(rDat, s.length());
						send(unoChat, Buffer, (s.length() + 12), 0);
						return;
					}
					Buffer = (char*)malloc(1024);
				}
			}
		}
	}
}