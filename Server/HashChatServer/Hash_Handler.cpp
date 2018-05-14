#include "Hash_Handler.h"



Hash_Handler::Hash_Handler()
{


}


Hash_Handler::~Hash_Handler()
{
}

int Hash_Handler::HashIndxr(std::string name)
{
	int h = NULL;

	for (int i = 0; i <= name.length(); i++)
	{
		h += static_cast<int>(name[i]);
	}

	return h % TABLE_SIZE;
}

int Hash_Handler::HashGrab(Data_Handler * _Handle, std::string name, SOCKET* s)
{
	int dex = HashIndxr(name);
	Data_Handler::UserData* ptr = _Handle->uDH[dex];

	while (ptr != NULL)
	{
		if (ptr->userName == name)
		{
			*s = ptr->s[1];
			return 1;
		}
		ptr = ptr->_uData;
	}

	return 0;
}
int Hash_Handler::HashGrab(Data_Handler * _Handle, std::string name, SOCKET s)
{
	int dex = HashIndxr(name);
	Data_Handler::UserData* ptr = _Handle->uDH[dex];

	while (ptr != NULL)
	{
		if (ptr->userName == name)
		{
			ptr->s[1] = s;
			return 1;
		}
		ptr = ptr->_uData;
	}

	return 0;
}

int Hash_Handler::HashGrab(Data_Handler * _Handle, std::string name)
{
	int dex = HashIndxr(name);
	Data_Handler::UserData* ptr = _Handle->uDH[dex];

	while (ptr != NULL)
	{
		if (ptr->userName == name)
		{
				return 1;
		}
		ptr = ptr->_uData;
	}

	return 0;
}

int Hash_Handler::HashGrab(Data_Handler * _Handle, std::string name, std::string pass)
{
	int dex = HashIndxr(name);
	Data_Handler::UserData* ptr = _Handle->uDH[dex];

	while (ptr != NULL)
	{
		if (ptr->userName == name)
		{
			if (ptr->passWord == pass) {
				ptr->Logged = TRUE;
				return 1;
			}
		}
		ptr = ptr->_uData;
	}

	return 0;
}



//int Hash_Handler::HashCheck(Data_Handler* _Handle, std::string name, std::string pass, SOCKET* s, int j)
//{
//	int dex = HashIndxr(name);
//	Data_Handler::UserData* ptr = _Handle->uDH[dex];
//	int I = NULL;
//	while (ptr != NULL)
//	{
//		if (ptr->userName == name)
//		{
//			I = 1;
//			break;
//		}
//		ptr = ptr->_uData;
//	}
//	switch (j)
//	{
//	case 0://check username
//		return I;
//	case 1:// check username and password, Logged in = TRUE;
//		if (!I) return I;
//		if (ptr->passWord == pass) {
//					ptr->Logged = TRUE;
//					return I;
//				}
//		break;
//	case 2:// check if logged in;
//		return ptr->Logged;
//		break;
//	case 3:// add socket data
//		if (!I) return I;
//		ptr->s[1] = *s;
//		break;
//	case 4:// return socket data
//		if (!I) return I;
//		*s = ptr->s[1];
//		return I;
//		break;
//	default:
//		break;
//	}
//
//	return 0;
//}

int Hash_Handler::HashMod(Data_Handler * _Handle, Data_Handler::UserData * udat)
{
	int dex = HashIndxr(udat->userName);
	Data_Handler::UserData* ptr = _Handle->uDH[dex];
	while (ptr != NULL)
	{
		if (ptr->userName == udat->userName) {
			ptr->passWord = udat->passWord;
			return 1;
		}
		ptr = ptr->_uData;
	}
	return 0;
}

int Hash_Handler::HashAdd(Data_Handler* _Handle, std::string name, std::string pass)
{
	int dex = HashIndxr(name);

	if ((_Handle->uDH[dex]->userName[0] == NULL))
	{
		_Handle->uDH[dex]->userName = name;
		_Handle->uDH[dex]->passWord = pass;
	}
	else
	{
		if (_Handle->uDH[dex]->userName == name)
			return 1;
		Data_Handler::UserData* ptr = _Handle->uDH[dex];
		while (ptr->_uData != NULL)
		{
			if (ptr->userName == name)
				return 1;
			ptr = ptr->_uData;
		}
		Data_Handler::UserData* tmp = new Data_Handler::UserData();
		tmp->userName = name;
		tmp->passWord = pass;
		tmp->_uData = NULL;

		ptr->_uData = tmp;

	}


	return 0;
}

void Hash_Handler::SaveData(Data_Handler * _Handle)
{
	std::fstream fs(FILE_NAME, std::fstream::out);
	Data_Handler::UserData* tmp = new Data_Handler::UserData();
	std::string encp("");
	std::string encn("");
	for (int i = NULL; i < TABLE_SIZE; i++) {
		if (!_Handle->uDH[i]->userName.empty())
		{
			tmp = _Handle->uDH[i];
			do {
				for (int i = 0; i < tmp->userName.length(); i++)
				{

					encn += tmp->userName[i] ^ KEY;
				}
				for (int i = 0; i < tmp->passWord.length(); i++)
				{
					encp += tmp->passWord[i] ^ KEY;
				}
				fs << encn << ":" << encp << ";";
				encp = "";
				encn = "";

				tmp = tmp->_uData;
			} while (tmp != NULL);
		}
	}
	fs.close();


}

void Hash_Handler::LoadData(Data_Handler * _Handle)
{
	std::fstream fs;
	fs.open(FILE_NAME, std::fstream::in);
	char t = NULL;
	std::string name;
	std::string pass;
	std::string encp("");
	std::string encn("");
	while (fs.peek() != EOF)
	{
		while (fs.peek() != ':') {
			fs.read(&t, 1);
			name += t;
		}
		fs.read(&t, 1);
		t = NULL;
		while (fs.peek() != ';') {
			fs.read(&t, 1);
			pass += t;
		}
		fs.read(&t, 1);
		t = NULL;
		for (int i = 0; i < name.length(); i++)
		{
			encn += name[i] ^ KEY;
		}
		for (int i = 0; i < pass.length(); i++)
		{
			encp += pass[i] ^ KEY;
		}
		HashAdd(_Handle, encn, encp);
		encp = "";
		encn = "";
		name = "";
		pass = "";

	}
	fs.close();
}